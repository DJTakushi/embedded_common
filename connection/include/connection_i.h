#pragma once
#include <condition_variable>
#include <functional>
#include <string>
#include "nlohmann/json.hpp"

namespace ec{
typedef std::function<void(std::string, void*)> message_callback;

class connection_i {
 public:
  virtual bool initialize() = 0;
  virtual void start_loop() = 0;
  virtual void stop_loop() = 0;
  virtual std::string get_received_message() = 0;
  virtual bool message_available() = 0;
  virtual void publish(std::string topic, std::string msg) = 0;
  virtual void close() = 0;
  virtual void subscriptions_add(std::string sub) = 0;
  virtual nlohmann::ordered_json gen_config() = 0;

  std::mutex mutex;
  std::condition_variable cv;
};
}//ec