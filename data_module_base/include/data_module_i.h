#pragma once
#include <string>
#include "connection_factory.h"

namespace ec{
class data_module_i{
 public:
  data_module_i(){};
  virtual void setup() = 0;
  virtual void start_all_threads() = 0;
  virtual void stop_all_threads() = 0;
  virtual void close() = 0;
  virtual bool is_active() = 0;
};
}//ec