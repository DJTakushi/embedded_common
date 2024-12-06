#pragma once

#include <nlohmann/json.hpp>
#include "connection_factory.h"

namespace ec{
class config_handler{
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
};
}//ec