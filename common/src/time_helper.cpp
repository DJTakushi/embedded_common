#include <ctime>
#include <chrono>
#include "time_helper.h"

std::string time_helper::time_rfc_3339() {
  /** https://en.cppreference.com/w/cpp/chrono/c/strftim */
  std::time_t time = std::time({});
  char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
  std::strftime(std::data(timeString), std::size(timeString),
                "%FT%TZ", std::gmtime(&time));
  return timeString;
}

uint64_t time_helper::get_epoch_now(){
  sys_tp time = std::chrono::system_clock::now();
  return(std::chrono::duration_cast<ms>(time.time_since_epoch()).count());
}
