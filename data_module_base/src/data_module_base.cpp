#include <iostream>
#include  "data_module_base.h"
data_module_base::data_module_base(std::string name,
                                    std::string pub_key,
                                    connection_type conn_type)
    : data_module_i(name,pub_key,conn_type),
    name_(name),
    publish_key_(pub_key),
    connection_type_(conn_type){
  local_conn_ = connection_factory::create(connection_type_);
}

void data_module_base::setup_local_conn(){
  local_conn_->initialize();
}

void data_module_base::local_publish(std::string topic, std::string data){
  local_conn_->publish(topic,data);
}

bool data_module_base::is_active(){
  return is_active_;
}

void data_module_base::receive_data_loop(){
  while(is_active_){
    receive_data();
  }
}

void data_module_base::update_data_loop(){
  while(is_active_){
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

void data_module_base::start_all_threads(){
  is_active_=true;
  receive_data_thread_ = std::thread([this](){
    this->receive_data_loop();
  });

  update_data_thread_ = std::thread([this](){
    this->update_data_loop();
  });
}

void data_module_base::stop_all_threads(){
  is_active_  =  false;
  if(receive_data_thread_.joinable()){
    receive_data_thread_.join();
  }
  if(update_data_thread_.joinable()){
    update_data_thread_.join();
  }
}