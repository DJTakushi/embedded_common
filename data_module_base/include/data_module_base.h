#pragma once
#include "connection_factory.h"

class data_module_base{
 protected:
  std::string publish_key_;
  connection_type connection_type_;
  std::shared_ptr<connection_i> local_conn_;
  bool is_active_{false};

  std::thread receive_data_thread_;
  void receive_data_loop();
  virtual void receive_data() = 0;
  std::mutex receive_data_mutex_;
  std::condition_variable receive_data_cv_;

  std::thread update_data_thread_; // update attributes & publish
  void update_data_loop();
  virtual void update_data() = 0;

  void publish_data();
  std::string generate_update_message();

  void setup_local_conn();
  void local_publish(std::string topic, std::string data);

 public:
  data_module_base(std::string pub_key, connection_type conn_type);
  virtual void setup() = 0;
  void start_all_threads();
  void stop_all_threads();
  virtual void close() = 0;

  bool is_active();
};