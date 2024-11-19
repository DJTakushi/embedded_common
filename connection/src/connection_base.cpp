#include "connection_base.h"

connection_base::connection_base(size_t max_messages){
  received_queue_ = std::make_shared<message_queue>(max_messages);
}

void connection_base::stop_loop(){
  active_ = false;
  if(do_work_thread_.joinable()){
    do_work_thread_.join();
  }
}

bool connection_base::message_available(){
  return received_queue_->get_queue_size() > 0;
}


std::string connection_base::get_received_message(){
  return received_queue_->get_popped_message();
};

void connection_base::add_message_to_queue(std::string msg){
  {
    std::unique_lock lk(mutex);
    received_queue_->add_message_to_queue(msg);
  }
  cv.notify_one();
}
