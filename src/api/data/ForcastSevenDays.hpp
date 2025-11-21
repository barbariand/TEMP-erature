#pragma once
#include <ArduinoJson.hpp>
#include <vector>
#include "Geometry.hpp"
#include "TimeSeriesData.hpp"
struct ForecastSevenDay {
  std::string createdTime;
  std::string referenceTime;
  Geometry geometry;
  std::vector<TimeSeriesItem> timeSeries;

  void fromJson(ArduinoJson::JsonDocument& doc);
};
