
#pragma once
#include <ArduinoJson.hpp>
#include <string>
#include <vector>
#include "../parameters/MeterologyCode.hpp"
#include "api/data/Station.hpp"
class LatestMonthsStations {
  MeterologyCode code;
  std::vector<Station> stations;

public:
  void fromJson(ArduinoJson::JsonDocument obj,MeterologyCode code);
};
