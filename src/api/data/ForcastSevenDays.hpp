#pragma once
#include <ArduinoJson.hpp>
#include <vector>
#include "Geometry.hpp"
#include "TimeSeriesItem.hpp"
struct ForecastSevenDay {
  std::string createdTime;
  std::string referenceTime;
  Geometry geometry;
  std::vector<TimeSeriesItem> timeSeries;

  void fromJson(ArduinoJson::JsonDocument& doc);
  std::vector<TimeSeriesItem> get_all_mid_day_reports();
};
