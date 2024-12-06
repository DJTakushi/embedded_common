#pragma once
#include "data_module_i.h"
#include "parser_i.h"
#include "attribute_host.h"

namespace ec{

enum data_module_status{
  kUninitialized,
  kConfiguring,
  kRunning,
  kExiting,
  kExited,
  kConfigBad
};
class data_module_base : public data_module_i{
 protected:
  data_module_status state_{kUninitialized};
  bool is_running();

  std::string name_;
  std::string publish_key_;
  attribute_host attribute_host_;
  std::shared_ptr<connection_i> local_conn_;

  void config_from_json(nlohmann::json j);

  std::queue<std::shared_ptr<void*>> received_data_; // void for cast-ability
  std::thread receive_data_thread_;
  void receive_data_loop();
  virtual void receive_data() = 0;
  std::mutex receive_data_mutex_;
  std::condition_variable receive_data_cv_;

  std::thread update_data_thread_; // update attributes & publish
  void update_data_loop();
  virtual void update_data() = 0;

  std::shared_ptr<parser_i> parser_;
  nlohmann::json create_update_message();
  void publish_data();

  void setup_local_conn();
  void local_publish(std::string topic, std::string data);

  void start_all_threads();
  void stop_all_threads();
 public:
  data_module_base(nlohmann::json config);
  virtual void setup() = 0;
  void start_running();
  virtual void exit() = 0;
  bool is_exited();
};
}//ec