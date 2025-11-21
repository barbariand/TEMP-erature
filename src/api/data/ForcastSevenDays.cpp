#include "ForcastSevenDays.hpp"
#include <ArduinoJson.hpp>
void ForecastSevenDay::fromJson(ArduinoJson::JsonDocument& doc) {
  createdTime = doc["createdTime"].as<std::string>();
  referenceTime = doc["referenceTime"].as<std::string>();

  geometry.fromJson(doc["geometry"]);

  timeSeries.clear();
  ArduinoJson::JsonArray seriesArray = doc["timeSeries"];
  for (ArduinoJson::JsonObject itemObj : seriesArray) {
    TimeSeriesItem item;
    item.fromJson(itemObj);
    timeSeries.push_back(item);
  }
}
