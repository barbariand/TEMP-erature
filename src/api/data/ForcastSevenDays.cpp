#include "ForcastSevenDays.hpp"
#include <ArduinoJson.hpp>

#include "TimeSeriesItem.hpp"
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
std::vector<TimeSeriesItem> ForecastSevenDay::get_all_mid_day_reports() {
    std::vector<TimeSeriesItem> out;
    out.reserve(7); // Prevent memory re-allocations for speed

    for (const auto& item : timeSeries) {
        if (item.time.hour == 12) {
            out.push_back(item);
            if (out.size() == 7) break;
        }
    }
    return out;
}
