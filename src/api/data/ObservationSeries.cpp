#include "ObservationSeries.hpp"
#include "ArduinoJson/Document/JsonDocument.hpp"
#include "api/parameters/MeterologyCode.hpp"
void ObservationSeries::fromJson(ArduinoJson::JsonDocument doc) {
  updated = SimpleDate::getLocalTimeFromUTCStamp(doc["updated"].as<int>());
  code = MeterologyCode(doc["parameter"]["key"].as<int>());

  ArduinoJson::JsonArray seriesArray = doc["value"];
  for (ArduinoJson::JsonObject itemObj : seriesArray) {
    DataPoint point;
    point.fromJson(itemObj);
    points.push_back(point);
  }
}

std::vector<DataPoint> ObservationSeries::getPoints() {
  return points;
}
