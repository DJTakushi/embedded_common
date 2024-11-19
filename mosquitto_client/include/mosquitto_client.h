#pragma once
#include <list>
#include <string>
#include <mosquitto.h>

class mosquitto_client {
  bool stable_{true}; // will be flagged false if problems detected
  bool log_mosquitto_{false};// TODO: set in constructor
  std::list<std::string> subscriptions_;
  std::string will_topic_;

  struct mosquitto *mosq_ {NULL};
  std::string mqtt_host_name_{"localhost"};
  uint mqtt_host_port_{1883};
  uint mqtt_host_keepalive_{60};

  void setup_mosquitto();

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
  mosquitto_client(std::string hostname,
                  uint mqtt_port,
                  uint mqtt_keepalive,
                  std::list<std::string> subscriptions,
                  std::string will_topic);
  bool is_stable();
  void publish(int* mid,
                const char* topic,
                int payloadlen,
                const void* payload,
                int qos,
                bool retain);
  void service_mqtt();
};