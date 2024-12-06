#pragma once
#include <string>
#include "connection_factory.h"

namespace ec{
class data_module_i{
 public:
  data_module_i(){};
  virtual void start_running() = 0;
  virtual void exit() = 0;
  virtual bool is_exited() = 0;
};
}//ec