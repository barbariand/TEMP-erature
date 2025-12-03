
#pragma once
#include <ArduinoJson.hpp>
#include <vector>
#include "DataPoint.hpp"
#include "api/data/Date.hpp"
#include "api/parameters/MeterologyCode.hpp"

class ObservationSeries {
  std::vector<DataPoint> points;
  MeterologyCode code;
  SimpleDate updated;

 public:
  void fromJson(ArduinoJson::JsonDocument doc);
};
