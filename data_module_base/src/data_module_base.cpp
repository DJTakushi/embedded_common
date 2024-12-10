#include <iostream>
#include <fstream>
#include "data_module_base_config.h"
#include "data_module_base.h"
namespace ec{
data_module_base::data_module_base(nlohmann::json config) :
                                                        config_queued_(config){
  state_ = kConfiguring;
}
void data_module_base::command_run(){
  run_commanded_=true;
}
void data_module_base::command_exit(){
  state_ = kExiting;
  run_commanded_ = false;
}
bool data_module_base::is_exited(){
  return state_ == kExited;
}

void data_module_base::state_machine(){
  switch(state_){
    case kUninitialized:
      break;
    case kConfiguring:
      stop();
      config_from_json(config_queued_);
      config_queued_.clear();
      save_generated_config();
      state_ = ec::kConfigured;
      break;
    case kConfigured:
      if(run_commanded_){
        state_ = kStarting;
      }
      break;
    case kStarting:
      start();
      break;
    case kRunning:
      break;
    case kExiting:
      exit();
      break;
    case kExited:
      /** NOTE: will not hit from state_machine_loop*/
      break;
    case kConfigBad:
      std::cerr << "config is bad ; cannot procede" << std::endl;
      state_ = kExiting;
      break;
    default:
      std::cerr << "unknown state : " << state_<<std::endl;
  }
}

void data_module_base::state_machine_loop(){
  while(!is_exited()){
    state_machine();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
void data_module_base::start_state_machine_loop(){
  state_machine_thread_ = std::thread([this](){
    this->state_machine_loop();
  });
}

bool data_module_base::is_config_good(nlohmann::json j){
  data_module_base_config cfg = data_module_base_config(j);
  return cfg.good;
}

void data_module_base::config_from_json(nlohmann::json j){
  data_module_base_config cfg = data_module_base_config(j);

  if(cfg.good){
    name_ = cfg.name;
    publish_key_ = cfg.pub_key;
    local_conn_ = connection_factory::create(cfg.type,cfg.address,cfg.port);
    parser_->configure(cfg.parser);

    nlohmann::json parser_attributes = parser_->get_all_supported_attributes();
    attribute_host_.update_attributes_from_array(parser_attributes);

    for(auto sub_key : cfg.sub_keys){
      local_conn_->subscriptions_add(sub_key);
    }
  }
  else{
    std::cerr << "config not good!" << std::endl;
    state_ = kConfigBad;
  }
}

bool data_module_base::is_running(){
  return state_ == kRunning || state_ == kStarting;
}
void data_module_base::setup_local_conn(){
  local_conn_->initialize();
}

void data_module_base::local_publish(std::string topic, std::string data){
  local_conn_->publish(topic,data);
}

void data_module_base::receive_data_loop(){
  while(is_running()){
    receive_data();
  }
}

void data_module_base::update_data_loop(){
  while(is_running()){
    update_data();
  }
}

nlohmann::json data_module_base::create_update_message(){
  steady_tp pub_time = std::chrono::steady_clock::now();

  nlohmann::ordered_json j;
  j["name"]=name_;
  for(auto attr : attribute_host_.get_updated_attributes()){
    nlohmann::json j_attr;
    j_attr["name"] = attr->get_name();
    j_attr["datatype"] = attr->get_datatype();
    j_attr["timestamp"] = attr->reported_epoch_get();

    size_t attr_size;
    uint64_t tahu_datatype;

    switch (attr->get_datatype()){
      case kInteger:
        j_attr["value"] = *((uint64_t*)(attr->get_value()));
        break;
      case kDouble:
        j_attr["value"] = *((double*)(attr->get_value()));
        break;
      case kString:
        j_attr["value"] = *((std::string*)(attr->get_value()));
        break;
      default:
        std::cout << "unknown attribute type!" << std::endl;
        break;
    }
    attr->update_time_published(pub_time);
    j["attributes"].emplace_back(j_attr);
  }

  return j;
}

void data_module_base::publish_data(){
  nlohmann::json j = create_update_message();
  if(j["attributes"].size()> 0){
    local_publish(publish_key_, j.dump());
  }
}

void data_module_base::rec_local_msg(std::string& msg){
  try{
    nlohmann::json jmsg = nlohmann::json::parse(msg);
    if(jmsg.contains("msg_type") && jmsg["msg_type"]=="config"){
      if(is_config_good(jmsg)){
        /** must configure elsewhere where this thread can be shut down */
        config_queued_ = jmsg;
        state_ = kConfiguring;
      }
    }
    else{
      std::cout << "unknown msg_type (?)" << std::endl;
    }
  }
  catch (nlohmann::json::exception& e) {
    std::cerr << "nlohmann::json::exception : " << e.what() << std::endl;
  }
}

void data_module_base::process_local_message_loop(){
  while(is_running()){
    std::string msg;
    {
      /* wait for lock message to be avaialble;
      lock mutex while poping message*/
      std::unique_lock lk(local_conn_->mutex);
      local_conn_->cv.wait(lk, [this] { return this->local_conn_->message_available(); });
      msg = local_conn_->get_received_message();
    }
    rec_local_msg(msg);
  }
}

void data_module_base::start_all_threads(){
  receive_data_thread_ = std::thread([this](){
    this->receive_data_loop();
  });

  update_data_thread_ = std::thread([this](){
    this->update_data_loop();
  });

  process_local_message_thread_ = std::thread([this](){
    this->process_local_message_loop();
  });
}

void data_module_base::stop_all_threads(){
  if(receive_data_thread_.joinable()){
    receive_data_thread_.join();
  }
  if(update_data_thread_.joinable()){
    update_data_thread_.join();
  }
  if(process_local_message_thread_.joinable()){
    process_local_message_thread_.join();
  }
}
void data_module_base::stop(){
  stop_all_threads();
  if(local_conn_.get() != NULL){
    local_conn_->close();
  }
}
void data_module_base::exit(){
  stop();
  state_ = ec::data_module_status::kExited;
}
void data_module_base::save_generated_config(){
  nlohmann::ordered_json new_config = config_gen();
  std::ofstream out("generated.json");
  out << new_config.dump(2);
  out.close();
}

nlohmann::ordered_json data_module_base::config_gen(){
  nlohmann::ordered_json config;
  config["name"] = name_;
  config["pub_key"] = publish_key_;

  config["local_conn"] = local_conn_->gen_config();
  config["hardware"] = config_hardware_gen();
  config["parser"] = parser_->get_config();

  return config;
}

}//ec
