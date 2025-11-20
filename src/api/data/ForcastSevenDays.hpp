#include <ArduinoJson.hpp>
#include <vector>
#include "Geometry.hpp"
#include "TimeSeriesData.hpp"
struct ForecastSevenDay {
  std::string createdTime;
  std::string referenceTime;
  Geometry geometry;
  std::vector<TimeSeriesItem> timeSeries;

  // Clear vector and load new data
  void fromJson(ArduinoJson::JsonDocument& doc) {
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
};
