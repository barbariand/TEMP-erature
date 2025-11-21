#include "TimeSeriesItem.hpp"
#include <ArduinoJson.hpp>
#include <ctime>
#include <iomanip>
#include <sstream>
time_t parseIsoTime(const std::string& isoString) {
  std::tm tm = {};
  std::istringstream ss(isoString);

  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

  if (ss.fail()) {
    return 0;
  }

  return mktime(&tm);
}
void TimeSeriesItem::fromJson(ArduinoJson::JsonObject obj) {
  std::string tmp = obj["time"].as<std::string>();
  time=SimpleDate::getLocalTimeFromIso(tmp);
  intervalStartTime = obj["intervalParametersStartTime"].as<std::string>();
  data.fromJson(obj["data"]);
}
