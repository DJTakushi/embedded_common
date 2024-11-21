#include  "data_module_base.h"
data_module_base::data_module_base(connection_type conn_type) :
    connection_type_(conn_type){}

void data_module_base::setup_local_conn(){
  local_conn_ = connection_factory::create(connection_type_);
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
