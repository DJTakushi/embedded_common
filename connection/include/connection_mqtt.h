#include <mosquitto.h>
#include <list>
#include "connection_base.h"

class connection_mqtt : public connection_base {
  bool stable_{true}; // will be flagged false if problems detected
  bool log_mosquitto_{false};// TODO: set in constructor
  std::list<std::string> subscriptions_;
  std::string will_topic_;


  struct mosquitto* mosq_;
  std::string host_{"localhost"};
  uint host_port_{1883};
  uint host_keep_alive_{60};

  bool is_active_{false};
  void service_mqtt();
  std::thread mosquitto_service_loop_thread_;
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
  void close();
};