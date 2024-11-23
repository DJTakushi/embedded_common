#pragma once
#include "nlohmann/json.hpp"
class parser_i{
  virtual void configure(nlohmann::json config) = 0;

  /** generate data from pointer to data-source*/
  virtual nlohmann::json get_data(void*) = 0;

  virtual nlohmann::json get_all_supported_attributes() = 0;

  virtual nlohmann::json get_config() = 0;
};