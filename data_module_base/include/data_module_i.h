#pragma once
#include <string>
#include "connection_factory.h"

class data_module_i{
 public:
  data_module_i(std::string name,
                std::string pub_key,
                connection_type conn_type){};
  virtual void setup() = 0;
  virtual void start_all_threads() = 0;
  virtual void stop_all_threads() = 0;
  virtual void close() = 0;
  virtual bool is_active() = 0;
};