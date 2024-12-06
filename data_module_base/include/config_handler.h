#pragma once

#include <nlohmann/json.hpp>
#include "connection_factory.h"

namespace ec{
class config_handler{
  static bool local_conn_object_exists(nlohmann::json j);
 public:
  static bool extract_name(nlohmann::json j, std::string& name);
  static bool extract_local_conf(nlohmann::json j, nlohmann::json& local_conf);
  static bool extract_local_conn_type(nlohmann::json j, connection_type& type);
  static bool extract_local_conn_address(nlohmann::json j,
                                          std::string& address);
  static bool extract_local_conn_port(nlohmann::json j, uint& port);
  static bool  extract_local_conn_pub_key(nlohmann::json j,
                                          std::string& pub_key);
  static bool extract_sub_keys(nlohmann::json j,
                                std::vector<std::string>& sub_keys);

  static bool set_local_conn_type(nlohmann::json& j, connection_type type);
  static bool set_local_conn_address(nlohmann::json& j, std::string address);
  static bool set_local_conn_port(nlohmann::json& j, uint port);
};
}//ec