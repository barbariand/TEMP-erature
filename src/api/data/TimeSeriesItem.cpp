#include "TimeSeriesItem.hpp"
#include <ArduinoJson.hpp>
#include <ctime>
void TimeSeriesItem::fromJson(ArduinoJson::JsonObject obj) {
  std::string tmp = obj["time"].as<std::string>();
  time=SimpleDate::getLocalTimeFromIso(tmp);
  intervalStartTime = obj["intervalParametersStartTime"].as<std::string>();
  data.fromJson(obj["data"]);
}
