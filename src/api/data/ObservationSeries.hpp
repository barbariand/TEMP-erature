
#pragma once
#include <ArduinoJson.hpp>
#include <vector>
#include "DataPoint.hpp"
#include "api/data/Date.hpp"
#include "api/parameters/MeterologyCode.hpp"

class ObservationSeries {
  MeterologyCode code;
  SimpleDate updated;

 public:
  std::vector<DataPoint> points;
  void fromJson(ArduinoJson::JsonDocument doc);
  std::vector<DataPoint> getPoints();
};
