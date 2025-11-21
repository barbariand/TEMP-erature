#include "api/data/TimeSeriesData.hpp"
void TimeSeriesItem::fromJson(ArduinoJson::JsonObject obj) {
  time = obj["time"].as<std::string>();
  intervalStartTime = obj["intervalParametersStartTime"].as<std::string>();
  data.fromJson(obj["data"]);
}
