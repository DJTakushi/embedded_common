#include <iostream>
#include "mosquitto_client.h"
mosquitto_client::mosquitto_client(std::string hostname,
                                    uint mqtt_port,
                                    uint mqtt_keepalive,
                                    std::list<std::string> subscriptions,
                                    std::string will_topic) :
                                    mqtt_host_name_(hostname),
                                    mqtt_host_port_(mqtt_port),
                                    mqtt_host_keepalive_(mqtt_keepalive),
                                    subscriptions_(subscriptions),
                                    will_topic_(will_topic) {
  setup_mosquitto();
}
bool mosquitto_client::is_stable(){
  return stable_;
}
void mosquitto_client::setup_mosquitto(){
  mosquitto_lib_init();
  mosq_ = mosquitto_new(NULL, true, this);
  if (mosq_) {
    mosquitto_log_callback_set(mosq_, mosquitto_client::log_callback);
    mosquitto_subscribe_callback_set(mosq_, mosquitto_client::subscribe_callback);
    mosquitto_connect_callback_set(mosq_, mosquitto_client::connect_callback);
    mosquitto_message_callback_set(mosq_, mosquitto_client::message_callback);
    mosquitto_username_pw_set(mosq_, "admin", "changeme"); // TODO : dynamic
    mosquitto_will_set(mosq_, will_topic_.c_str(), 0, NULL, 0, false);

    if (mosquitto_connect(mosq_,
                          mqtt_host_name_.c_str(),
                          mqtt_host_port_,
                          mqtt_host_keepalive_) == 0) {
      std::cout << "mosquitto connection established" <<std::endl;
    }
    else{
      std::cerr << "Unable to connect."<<std::endl;
      stable_ = false;
    }
  }
  else{
    std::cerr << "Error creating mosquitto handler"<<std::endl;
    stable_ = false;
  }
}

void mosquitto_client::connect_callback(struct mosquitto *mosq,void *userdata,int result) {
  mosquitto_client* mosquitto_client_ = (mosquitto_client*)userdata;
  if (!result) {
    for(auto sub : mosquitto_client_->subscriptions_){
      mosquitto_subscribe(mosq, NULL, sub.c_str(), 0);
    }
  } else {
    std::cerr << "MQTT Connect failed : code " << result << std::endl;
  }
}
void mosquitto_client::subscribe_callback(struct mosquitto *mosq,
                              void *userdata,
                              int mid,
                              int qos_count,
                              const int *granted_qos){
  std::cout << "subscribed (mid: "<< mid << "): "<< granted_qos[0];;
  for (int i = 1; i < qos_count; i++) {
    std::cout << ", " << granted_qos[i];
  }
  std::cout << std::endl;
}
void mosquitto_client::log_callback(struct mosquitto *mosq,
                        void *userdata,
                        int level,
                        const char *str) {
  // Print all log messages regardless of level.
  mosquitto_client* mosquitto_client_ = (mosquitto_client*)(userdata);
  if(mosquitto_client_->log_mosquitto_){
    std::cout << str <<  std::endl;
  }
}
void mosquitto_client::message_callback(struct mosquitto *mosq,
                                  void *userdata,
                                  const struct mosquitto_message *message){
  /* TODO: implement a queue of messages to add this to*/
}

void mosquitto_client::publish(int* mid,
                                const char* topic,
                                int payloadlen,
                                const void* payload,
                                int qos,
                                bool retain){
  mosquitto_publish(mosq_,mid,topic,payloadlen,payload,qos,retain);
}
void mosquitto_client::service_mqtt(){
  mosquitto_loop(mosq_, 0, 1);
}