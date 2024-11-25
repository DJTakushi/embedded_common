#pragma once
#include <chrono>
#include <string>
#include <sys/types.h>
typedef std::chrono::system_clock::time_point sys_tp;
typedef std::chrono::milliseconds ms;

class time_helper{
 public:
  static std::string time_rfc_3339();
  static uint64_t get_epoch_now();
};