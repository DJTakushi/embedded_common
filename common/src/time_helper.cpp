#include <ctime>
#include "time_helper.h"

std::string time_helper::time_rfc_3339() {
  /** https://en.cppreference.com/w/cpp/chrono/c/strftim */
  std::time_t time = std::time({});
  char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
  std::strftime(std::data(timeString), std::size(timeString),
                "%FT%TZ", std::gmtime(&time));
  return timeString;
}
