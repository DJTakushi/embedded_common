#pragma once
#include "connection_factory.h"

class data_module_base{
 protected:
  connection_type connection_type_;
  std::shared_ptr<connection_i> local_conn_;
  bool is_active_{false};
  std::thread work_loop_thread_;
  virtual void work_loop() = 0;
  void setup_local_conn();
  void local_publish(std::string topic, std::string data);

 public:
  data_module_base(connection_type conn_type);
  virtual void setup() = 0;
  void start_work_loop();
  virtual void close() = 0;

  bool is_active();
};