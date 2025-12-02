#pragma once
#include <ArduinoJson.hpp>
#include <string>
class Station {
  std::string name;
public:
  void fromJson(ArduinoJson::JsonObject obj);
};
