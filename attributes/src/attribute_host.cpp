#include <iostream>
#include "attribute_host.h"
attribute_host::attribute_host(){
  // TODO :
}

void attribute_host::update_attribute(nlohmann::json& j, steady_tp time){
  std::cout << "update_attribute attr : " << j.dump() <<  std::endl;

  if(j.contains("name") && j.contains("datatype")){
    std::string attr_name = j["name"];
    uint64_t attr_datatype = j["datatype"];
    if(attributes_.find(attr_name)==attributes_.end()){
      std::cout << "creating attribute : "<< attr_name <<"..."<<std::endl;
      attributes_[attr_name] = std::make_shared<attribute>(attr_name,
                                                            attr_datatype);
    }
    else{
      // do nothing?
    }

    /** TODO : should timestamp be updated if it is included?
    assume all should be updated (for now[!])  */
    if(j.contains("value") && j.contains("timestamp")){
      if(j["timestamp"] != attributes_[attr_name]->reported_epoch_get()){
        attributes_[attr_name]->set_value_with_timetamp(j["value"],time);
        attributes_[attr_name]->reported_epoch_set(j["timestamp"]);
      }
    }
    else{
      std::cout << "missing 'value' and/or 'timestamp'" << std::endl;
    }
  }
  else{
    std::cout << "name or datatype not found in json : "<<j.dump()<<std::endl;
  }
}

void attribute_host::update_attributes_from_array(nlohmann::json& j){
  steady_tp now = std::chrono::steady_clock::now();
  if(j.is_array()){
    for(auto it = j.begin(); it != j.end(); ++it){
      update_attribute(*it,now);
    }
  }
  else{
    std::cout << "json not an array : " << j.dump()<<std::endl;
  }
}
attribute_vector attribute_host::get_updated_attributes(){
  attribute_vector out;
  for(auto attr : attributes_){
    if(attr.second->is_recently_published()){
      out.push_back(attr.second);
    }
  }
  return out;
}