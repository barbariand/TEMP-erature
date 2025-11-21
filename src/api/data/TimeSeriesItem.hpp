
#pragma once
#include <ArduinoJson.hpp>
#include <string>
#include "Date.hpp"
#include "ForcastData.hpp"
struct TimeSeriesItem {
  SimpleDate time;
  std::string intervalStartTime;
  ForecastData data;
  TimeSeriesItem(){};
  void fromJson(JsonObject obj);
};
