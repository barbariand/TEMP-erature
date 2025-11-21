
#pragma once
#include <string>
#include <ArduinoJson.hpp>
#include "ForcastData.hpp"
struct TimeSeriesItem {
  std::string time;
  std::string intervalStartTime;
  ForecastData data;

  void fromJson(JsonObject obj);
};
