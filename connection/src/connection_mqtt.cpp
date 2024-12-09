#include <iostream>
#include "connection_mqtt.h"

namespace ec{
void connection_mqtt::connect_callback(struct mosquitto *mosq,
                                      void *userdata,
                                      int result) {
  connection_mqtt* connection_mqtt_ = (connection_mqtt*)userdata;
  if (!result) {
    // Subscribe to commands
    for(auto sub : connection_mqtt_->subscriptions_){
      mosquitto_subscribe(mosq, NULL, sub.c_str(), 0);
    }
  }
  else {
    std::cerr << "MQTT Connect failed" << std::endl;
  }
}
void connection_mqtt::subscribe_callback(struct mosquitto *mosq,
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
void connection_mqtt::log_callback(struct mosquitto *mosq,
                                  void *userdata,
                                  int level,
                                  const char *str) {
  // Print all log messages regardless of level.
  connection_mqtt* connection_mqtt_ = (connection_mqtt*)(userdata);
  if(connection_mqtt_->log_mosquitto_){
    std::cout << str <<  std::endl;
  }
}
void connection_mqtt::message_callback(struct mosquitto *mosq,
                                      void *userdata,
                                      const struct mosquitto_message *message) {
  std::string payload((char *)(message->payload),message->payloadlen);
  connection_mqtt* connection = (connection_mqtt*)userdata;
  connection->add_message_to_queue(payload);
}

connection_mqtt::connection_mqtt(std::string host,
                                  int port,
                                  size_t max_messages)
    : connection_base(max_messages), host_(host), host_port_(port){
}

bool connection_mqtt::initialize(){
  bool stable_ = false;
  mosquitto_lib_init();
  bool clean_session = true;
  mosq_ = mosquitto_new(NULL, clean_session, this);
  if (mosq_ != NULL) {
    mosquitto_log_callback_set(mosq_, connection_mqtt::log_callback);
    mosquitto_subscribe_callback_set(mosq_, connection_mqtt::subscribe_callback);
    mosquitto_connect_callback_set(mosq_, connection_mqtt::connect_callback);
    mosquitto_message_callback_set(mosq_, connection_mqtt::message_callback);
    mosquitto_username_pw_set(mosq_, "admin", "changeme");
    if(will_topic_.size()>0){
      mosquitto_will_set(mosq_, will_topic_.c_str(), 0, NULL, 0, false);
    }

    std::cout << "connecting to host " << host_ << " ; port " << host_port_ << std::endl;
    if (mosquitto_connect(mosq_,
                          host_.c_str(),
                          host_port_,
                          host_keep_alive_) == 0) {
      stable_ = true;
      std::cout << "mosquitto connection established" <<std::endl;
      start_loop();
    }
    else{
      std::cerr<< "Unable to connect to MQTT."<<std::endl;
      stable_ = false;
    }
  }
  else {
    std::cerr << "Error creating mosquitto handler"<<std::endl;
    stable_ = false;
  }
  return stable_;
}
void connection_mqtt::service_mqtt(){
  mosquitto_loop(mosq_, 0, 1);
}
void connection_mqtt::mosquitto_service_loop(){
  while(connection_active_){
    service_mqtt();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "mosquitto_service_loop exiting..."<<std::endl;
}
void connection_mqtt::start_loop(){
  connection_active_=true;
  do_work_thread_ = std::thread([this](){
    this->mosquitto_service_loop();
  });
}

void connection_mqtt::publish(std::string topic, std::string msg){
  mosquitto_publish(mosq_, NULL, topic.c_str(), msg.size(), msg.c_str(), 0, false);
}
void connection_mqtt::publish(int* mid,
                                const char* topic,
                                int payloadlen,
                                const void* payload,
                                int qos,
                                bool retain){
  mosquitto_publish(mosq_,mid,topic,payloadlen,payload,qos,retain);
}
void connection_mqtt::set_will_topic(std::string topic){
  will_topic_ = topic;
}
void connection_mqtt::close(){
  stop_loop();
  mosquitto_disconnect(mosq_);
}
bool connection_mqtt::is_stable(){
  return stable_;
}
nlohmann::ordered_json connection_mqtt::gen_config(){
  nlohmann::ordered_json config;
  config["type"] = "MQTT";
  config["address"] = host_;
  config["port"] = host_port_;

  config["sub_keys"] = subscriptions_;

  return config;
}

}//ec