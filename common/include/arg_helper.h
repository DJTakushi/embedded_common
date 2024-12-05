#include "connection_factory.h"

namespace ec{
class arg_helper{
 public:
  static connection_type get_connection_type(int argc, char* argv[]);
  static std::string get_address(int argc, char* argv[]);
  static uint get_port(int argc, char* argv[]);
  static std::string get_spb_address(int argc, char* argv[]);
};
}//ec