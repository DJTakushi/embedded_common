#pragma once
#include <sys/types.h>
#include <map>
#include <mutex>
#include <string>
#include "nlohmann/json.hpp"
#include "attribute.h"

namespace ec{
typedef std::map<std::string,std::shared_ptr<attribute>> attribute_map;
typedef std::vector<std::shared_ptr<attribute>> attribute_vector;
class attribute_host{
  attribute_map attributes_;
 public:
  std::mutex attribute_mutex;
  void update_attribute(nlohmann::json& j, steady_tp time);
  void update_attributes_from_array(nlohmann::json& j);
  attribute_vector get_updated_attributes();
};
}//ec