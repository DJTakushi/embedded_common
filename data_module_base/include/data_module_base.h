#pragma once
#include "data_module_i.h"
#include "parser_i.h"
#include "attribute_host.h"

namespace ec{

enum data_module_status{
  kUninitialized,
  kConfiguring,
  kConfigured,
  kSettingUp,
  kRunning,
  kExiting,
  kExited,
  kConfigBad
};
class data_module_base : public data_module_i{
 protected:
  nlohmann::json config_queued_;
  data_module_status state_{kUninitialized};
  bool is_running();
  bool run_commanded_{false};

  void state_machine();
  std::thread state_machine_thread_;
  void state_machine_loop();
  void start_state_machine_loop();

  std::string name_;
  std::string publish_key_;
  attribute_host attribute_host_;
  std::shared_ptr<connection_i> local_conn_;

  virtual bool is_config_good(nlohmann::json j);
  virtual void config_from_json(nlohmann::json j);

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

  nlohmann::ordered_json config_gen();
  void save_generated_config();

  void setup_local_conn();
  void local_publish(std::string topic, std::string data);
  std::thread process_local_message_thread_;
  void rec_local_msg(std::string& msg);
  void process_local_message_loop();

  void start_all_threads();
  void stop_all_threads();

  virtual void setup() = 0;
  virtual void stop() = 0;
  void exit();
 public:
  data_module_base(nlohmann::json config);
  void command_run();
  void command_exit();
  bool is_exited();
};
}//ec