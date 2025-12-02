#include "LatestMonthsStation.hpp"
#include "api/data/Station.hpp"
void LatestMonthsStations::fromJson(ArduinoJson::JsonDocument doc,
                                    MeterologyCode code) {
  this->code = code;

  ArduinoJson::JsonArray seriesArray = doc["station"];
  for (ArduinoJson::JsonObject itemObj : seriesArray) {
    Station station;
    station.fromJson(itemObj);
    stations.push_back(station);
  }
}
