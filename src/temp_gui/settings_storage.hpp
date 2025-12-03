#pragma once

#include <ArduinoJson.h>
#include <string>
#include "api/cities.hpp"
#include "api/parameters/MeterologyCode.hpp"

struct Settings {
  int city = kKnownCities[0].station;
  MeterologyCode parameter = MeterologyCode::AirTemperature_DailyMean;
};

namespace SettingsStorage {
Settings load(bool retry = false);
bool save(const Settings& s);
}  // namespace SettingsStorage
