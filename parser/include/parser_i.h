#pragma once
#include "nlohmann/json.hpp"
namespace ec{
class parser_i{
 public:
  virtual void configure(nlohmann::json config) = 0;

  /** generate data from pointer to data-source*/
  virtual nlohmann::json get_attributes_from_data(void* data,
                                                  uint64_t epoch) = 0;

  virtual nlohmann::json get_all_supported_attributes() = 0;

  virtual nlohmann::ordered_json get_config() = 0;
};
}//ec