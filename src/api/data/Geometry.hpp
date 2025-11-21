
#pragma once
#include <ArduinoJson.hpp>
struct Geometry {
  std::string type;
  double lat;
  double lon;

  void fromJson(ArduinoJson::JsonObject obj);
};
