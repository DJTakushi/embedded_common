#include <iostream>
#include "argument_helper.h"
connection_type argument_helper::get_connection_type(int argc, char* argv[]){
  connection_type type = kMqtt;
  for(size_t i = 1; i < argc; i++) {
    std::string str(argv[i]);
    if (str.substr(0,2)=="-c"){
      if(str.substr(2,str.npos)==AZURE_ROUTES){
        type = kAzureIot;
        std::cout << "connecion type '"<<AZURE_ROUTES<<"' detected"<<std::endl;
      }
      if(str.substr(2,str.npos)==MQTT){
        type = kMqtt;
        std::cout << "connecion type '"<<MQTT<<"' detected"<<std::endl;
      }
    }
  }
  return type;
};
std::string argument_helper::get_address(int argc, char* argv[]){
  std::string address = "localhost";
  for(size_t i = 1; i < argc; i++){
      std::string str(argv[i]);
    if (str.substr(0,2)=="-a"){
      address = str.substr(2,str.npos);
    }
  }
  return address;
}
uint argument_helper::get_port(int argc, char* argv[]){
  uint port = 1883;
  for(size_t i = 1; i < argc; i++){
      std::string str(argv[i]);
    if (str.substr(0,2)=="-p"){
      port = std::stoi(str.substr(2,str.npos));
    }
  }
  return port;
}
