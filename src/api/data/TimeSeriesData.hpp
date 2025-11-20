#include <string>
#include "ForcastData.hpp"
struct TimeSeriesItem {
  std::string time;
  std::string intervalStartTime;
  ForecastData data;

  void fromJson(JsonObject obj) {
    time = obj["time"].as<std::string>();
    intervalStartTime = obj["intervalParametersStartTime"].as<std::string>();
    data.fromJson(obj["data"]);
  }
};
