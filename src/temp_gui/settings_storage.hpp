#pragma once

#include <string>
#include "api/parameters/MeterologyCode.hpp"
#include <ArduinoJson.h>

struct Settings {
  std::string city;
  MeterologyCode parameter = MeterologyCode::AirTemperature_DailyMean; // e.g. "Temperature", "Humidity", "Wind Speed"
};

namespace SettingsStorage {
Settings load();
bool save(const Settings& s);
} // namespace SettingsStorage
