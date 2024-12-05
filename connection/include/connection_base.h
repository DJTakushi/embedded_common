#pragma once
#include <list>
#include <memory>
#include <queue>
#include <thread>
#include "connection_i.h"
#include "message_queue.h"

namespace ec{
class connection_base : public connection_i {
 protected:
  std::shared_ptr<message_queue> received_queue_;
  bool active_{false};
  std::thread do_work_thread_;
  std::list<std::string> subscriptions_;
 public:
  connection_base(size_t max_messages);
  void stop_loop();
  bool message_available();
  std::string get_received_message();
  void add_message_to_queue(std::string msg);
  void subscriptions_add(std::string sub);
};
}//ec