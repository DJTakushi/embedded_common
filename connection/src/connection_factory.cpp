#include <iostream>
#include "connection_factory.h"

namespace ec{
std::shared_ptr<connection_i> connection_factory::create(connection_type type,
                                                          std::string host,
                                                          uint port) {
  std::shared_ptr<connection_i> out;
  switch(type){
    case kAzureIot:
      std::cout << "creating azure iot connection..."<<std::endl;
      out = std::make_shared<connection_azure_routes>(100);
      break;
    case kMqtt:
    default:
      std::cout << "creating mqtt connection..."<<std::endl;
      out = std::make_shared<connection_mqtt>(host,port,100);
  }
  return out;
}
}//ec