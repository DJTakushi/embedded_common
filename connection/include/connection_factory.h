#pragma once
#include <memory>

#include "connection_azure_routes.h"
#include "connection_mqtt.h"

#define AZURE_ROUTES "AZURE_ROUTES"
#define MQTT "MQTT"

namespace ec{
enum connection_type {
  kAzureIot,
  kMqtt
};

class connection_factory {
 public:
  static std::shared_ptr<connection_i> create(connection_type type,
                                              std::string address,
                                              uint port);
};
}//ec