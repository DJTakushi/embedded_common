#pragma once
#include <mosquitto.h>
#include <sys/types.h>
#include "connection_base.h"

namespace ec{
class connection_mqtt : public connection_base {
  bool stable_{true}; // will be flagged false if problems detected
  bool log_mosquitto_{false};// TODO: set in constructor
  std::string will_topic_;

  struct mosquitto* mosq_;
  std::string host_{"localhost"};
  uint host_port_{1883};
  uint host_keep_alive_{60};

  void service_mqtt();
  void mosquitto_service_loop();

  static void connect_callback(struct mosquitto *mosq,
                                  void *userdata,
                                  int result);
  static void subscribe_callback(struct mosquitto *mosq,
                                    void *userdata,
                                    int mid,
                                    int qos_count,
                                    const int *granted_qos);
  static void log_callback(struct mosquitto *mosq,
                              void *userdata,
                              int level,
                              const char *str);
  static void message_callback(struct mosquitto *mosq,
                                  void *userdata,
                                  const struct mosquitto_message *message);
 public:
  connection_mqtt(std::string host,int port,size_t max_messages);
  bool initialize();
  void start_loop();
  void publish(std::string topic, std::string msg);
  void publish(int* mid,
                const char* topic,
                int payloadlen,
                const void* payload,
                int qos,
                bool retain);
  void close();
  void set_will_topic(std::string topic);
  bool is_stable();
};
}//ec