#pragma once
#include <string>
#include "connection_factory.h"

namespace ec{
class data_module_i{
 public:
  data_module_i(std::string name,
                std::string pub_key,
                connection_type conn_type,
                std::string address,
                uint port){};
  virtual void setup() = 0;
  virtual void start_all_threads() = 0;
  virtual void stop_all_threads() = 0;
  virtual void close() = 0;
  virtual bool is_active() = 0;
};
}//ec