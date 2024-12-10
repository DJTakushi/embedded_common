#pragma once
#include <nlohmann/json.hpp>
#include "data_module_i.h"

namespace ec{
struct data_module_base_config{
  bool good{false};
  std::string              name;
  connection_type          type;
  std::string              address;
  uint                     port;
  std::string              pub_key;
  std::vector<std::string> sub_keys;
  nlohmann::json           parser;
  data_module_base_config(nlohmann::json j);

  static bool local_conn_object_exists(nlohmann::json j);
  static bool extract_name(nlohmann::json j, std::string& name);
  static bool extract_local_conf(nlohmann::json j, nlohmann::json& local_conf);
  static bool extract_local_conn_type(nlohmann::json j, connection_type& type);
  static bool extract_local_conn_address(nlohmann::json j,
                                          std::string& address);
  static bool extract_local_conn_port(nlohmann::json j, uint& port);
  static bool  extract_pub_key(nlohmann::json j,
                                std::string& pub_key);
  static bool extract_sub_keys(nlohmann::json j,
                                std::vector<std::string>& sub_keys);
  static bool extract_parser(nlohmann::json j, nlohmann::json& parser);

  static bool set_local_conn_type(nlohmann::json& j, connection_type type);
  static bool set_local_conn_address(nlohmann::json& j, std::string address);
  static bool set_local_conn_port(nlohmann::json& j, uint port);
};
}//ec