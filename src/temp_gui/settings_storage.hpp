#pragma once

#include <ArduinoJson.h>
#include <string>
#include "api/cities.hpp"
#include "api/parameters/MeterologyCode.hpp"

struct Settings {
  std::string city = kKnownCities[0].name;
  MeterologyCode parameter = MeterologyCode::
      AirTemperature_DailyMean;  // e.g. "Temperature", "Humidity", "Wind Speed"
};

namespace SettingsStorage {
Settings load();
bool save(const Settings& s);
}  // namespace SettingsStorage
