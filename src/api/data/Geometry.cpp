
#include "Geometry.hpp"
void Geometry::fromJson(ArduinoJson::JsonObject obj) {
  type = obj["type"].as<std::string>();
  ArduinoJson::JsonArray coords = obj["coordinates"];
  lon = coords[0];
  lat = coords[1];
}
