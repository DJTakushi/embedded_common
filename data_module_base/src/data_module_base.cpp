#include  "data_module_base.h"
data_module_base::data_module_base(std::string pub_key, connection_type conn_type) :
    publish_key_(pub_key),
    connection_type_(conn_type){}

void data_module_base::setup_local_conn(){
  local_conn_ = connection_factory::create(connection_type_);
  local_conn_->initialize();
}

void data_module_base::start_work_loop(){
  is_active_=true;
  work_loop_thread_ = std::thread([this](){
    this->work_loop();
  });
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

void data_module_base::publish_data(){
  // TODO : check if data is worth updating
  std::string msg =  generate_update_message();
  local_publish(publish_key_, msg);
}

std::string data_module_base::generate_update_message(){
  std::string out;
  // TODO
  return out;
}
