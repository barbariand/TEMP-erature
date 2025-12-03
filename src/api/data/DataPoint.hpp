#pragma once
#include <ArduinoJson.h>
#include "api/data/TimeSeriesItem.hpp"
using ArduinoJson::JsonObject;
struct DataPoint {
  SimpleDate date;
  float value;

 public:
  void fromJson(JsonObject obj);
};
