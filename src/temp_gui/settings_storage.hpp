#pragma once

#include <string>
#include <ArduinoJson.h>

struct Settings {
  std::string city;
  std::string parameter = "Temperature"; // e.g. "Temperature", "Humidity", "Wind Speed"
};

namespace SettingsStorage {
Settings load();
bool save(const Settings& s);
} // namespace SettingsStorage
