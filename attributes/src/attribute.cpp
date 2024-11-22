#include <sys/types.h>
#include "attribute.h"
attribute::attribute(std::string name, uint64_t datatype)
                                  : name_(name),
                                    datatype_(datatype) {
  switch (datatype_){
    case kInteger:
      value_ = (void*)(new uint64_t);
      break;
    case kDouble:
      value_ = (void*)(new double);
      break;
    case kString:
      value_ = (void*)(new std::string);
      break;


  }}
void attribute::set_value_with_timetamp(nlohmann::json& j_val,
                                              steady_tp time){
  void* data = NULL;
  switch (datatype_){
    case kInteger:
      if (j_val.is_number_integer()){
        *((uint64_t*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
    case kDouble:
      if (j_val.is_number_float()){
        *((double*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
    case kString:
      if (j_val.is_string()){
        *((std::string*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
  }
}

const void* attribute::get_value(){
  return value_;
}
uint64_t attribute::get_datatype(){
  return datatype_;
}
bool attribute::is_recently_published(){
  return time_sent_ < time_recv_;
}
uint64_t attribute::reported_epoch_get(){
  return reported_epoch_;
}

void attribute::reported_epoch_set(uint64_t epoch){
  reported_epoch_ = epoch;
}

std::string attribute::get_name(){
  return name_;
}