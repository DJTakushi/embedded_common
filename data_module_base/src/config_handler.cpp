#include <iostream>
#include "config_handler.h"
namespace ec{
bool config_handler::extract_name(nlohmann::json j, std::string& name){
  bool success = false;
  if(j.contains("name")){
    if(j["name"].is_string()){
      name = j["name"];
      success = true;
    }
    else{
      std::cerr << "config name is not string" << std::endl;
    }
  }
  else{
    std::cerr << "name not in config" << std::endl;
  }
  return success;
}

bool config_handler::local_conn_object_exists(nlohmann::json j){
  return j.contains("local_conn") && j["local_conn"].is_object();
}

bool config_handler::extract_local_conf(nlohmann::json j,
                                        nlohmann::json& local_conf){
  bool success = false;
  if(j.contains("local_conn")){
    if(j["local_conn"].is_object()){
      local_conf = j["local_conn"];
      success = true;
    }
    else{
      std::cerr << "config.local_conn is not an object" << std::endl;
    }
  }
  else{
    std::cerr << "config.local_conn is missing" << std::endl;
  }
  return success;
}

bool config_handler::extract_local_conn_type(nlohmann::json j, connection_type& type){
  bool success = false;
  nlohmann::json local_conn;
  if(extract_local_conf(j, local_conn)){
    success = false;
    if(local_conn.contains("type")){
      if(local_conn["type"].is_string()){
        if(local_conn["type"]==MQTT){
          type = kMqtt;
          success = true;
        }
        else if(local_conn["type"]==AZURE_ROUTES){
          type = kAzureIot;
          success = true;
        }
        else{
          std::cerr << "config.local_conn.type unknown"<<std::endl;
        }
      }
      else{
        std::cerr << "config.local_conn.type is not a string"<<std::endl;
      }
    }
    else{
      std::cerr << "config.local_conn.type is missing" << std::endl;
    }
  }
  return success;
}


bool config_handler::extract_local_conn_address(nlohmann::json j, std::string& address){
  bool success = false;
  nlohmann::json local_conn;
  if(extract_local_conf(j, local_conn)){
    success = false;
    if(local_conn.contains("address")){
      if(local_conn["address"].is_string()){
        address = local_conn["address"];
        success = true;
      }
      else{
        std::cerr << "config.local_conn.address is not a string"<<std::endl;
      }
    }
    else{
      std::cerr << "config.local_conn.address is missing" << std::endl;
    }
  }
  return success;
}
bool config_handler::extract_local_conn_port(nlohmann::json j, uint& port){
  bool success = false;
  nlohmann::json local_conn;
  if(extract_local_conf(j, local_conn)){
    success = false;
    if(local_conn.contains("port")){
      if(local_conn["port"].is_number_integer()){
        port = local_conn["port"];
        success = true;
      }
      else{
        std::cerr << "config.local_conn.port is not a string"<<std::endl;
      }
    }
    else{
      std::cerr << "config.local_conn.port is missing" << std::endl;
    }
  }
  return success;
}
bool  config_handler::extract_local_conn_pub_key(nlohmann::json j, std::string& pub_key){
  bool success = false;
  nlohmann::json local_conn;
  if(extract_local_conf(j, local_conn)){
    success = false;
    if(local_conn.contains("pub_key")){
      if(local_conn["pub_key"].is_string()){
        pub_key = local_conn["pub_key"];
        success = true;
      }
      else{
        std::cerr << "config.local_conn.pub_key is not a string"<<std::endl;
      }
    }
    else{
      std::cerr << "config.local_conn.pub_key is missing" << std::endl;
    }
  }
  return success;
}
bool config_handler::extract_sub_keys(nlohmann::json j,
                                      std::vector<std::string>& sub_keys){
  bool success = false;
  nlohmann::json local_conn;
  if(extract_local_conf(j, local_conn)){
    success = false;
    if(local_conn.contains("sub_keys")){
      if(local_conn["sub_keys"].is_array()){
        nlohmann::json sub_key_j = local_conn["sub_keys"];
        for(auto it = sub_key_j.begin(); it < sub_key_j.end(); ++it){
          if(it->is_string()){
            sub_keys.push_back(*it);
            success = true;
          }
          else{
            std::cerr << "sub_key not a string :  "<< it->dump() <<std::endl;
          }
        }
      }
      else{
        std::cerr << "config.local_conn.sub_keys is not an array"<<std::endl;
      }
    }
    else{
      std::cerr << "config.local_conn.sub_keys is missing" << std::endl;
    }
  }
  return success;
}

bool config_handler::set_local_conn_type(nlohmann::json& j,
                                          connection_type type){
  bool success = false;
  if(local_conn_object_exists(j)){
    if(j["local_conn"].contains("type")){
      if(j["local_conn"]["type"].is_string()){
        switch (type){
          case kAzureIot:
            j["local_conn"]["type"] = AZURE_ROUTES;
            success = true;
            break;
          case kMqtt:
            j["local_conn"]["type"] = MQTT;
            success = true;
            break;
          default:
            std::cerr << "set_local_conn_type unknown type : ";
            std::cerr << type << std::endl;
        }
      }
      else{
        std::cerr << "type not string"  << std::endl;
      }
    }
    else{
      std::cerr << "type not found" <<  std::endl;
    }
  }
  else{
    std::cerr << "unable to extract local conf" << std::endl;
  }
  return success;
}

bool config_handler::set_local_conn_address(nlohmann::json& j,
                                            std::string address){
  bool success = false;
  if(local_conn_object_exists(j)){
    if(j["local_conn"].contains("address")){
      if(j["local_conn"]["address"].is_string()){
        j["local_conn"]["address"] = address;
        success = true;
      }
      else{
        std::cerr << "address not string"  << std::endl;
      }
    }
    else{
      std::cerr << "address not found" <<  std::endl;
    }
  }
  else{
    std::cerr << "unable to extract local conf" << std::endl;
  }
  return success;
}

bool config_handler::set_local_conn_port(nlohmann::json& j, uint port){
  bool success = false;
  if(local_conn_object_exists(j)){
    if(j["local_conn"].contains("port")){
      if(j["local_conn"]["port"].is_number_integer()){
        j["local_conn"]["port"] = port;
        success = true;
      }
      else{
        std::cerr << "port not integer"  << std::endl;
      }
    }
    else{
      std::cerr << "port not found" <<  std::endl;
    }
  }
  else{
    std::cerr << "unable to extract local conf" << std::endl;
  }
  return success;
}
}//ec