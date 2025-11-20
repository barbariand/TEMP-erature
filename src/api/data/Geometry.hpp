#include <ArduinoJson.hpp>
struct Geometry {
  std::string type;
  double lat;
  double lon;

  void fromJson(ArduinoJson::JsonObject obj) {
    type = obj["type"].as<std::string>();
    ArduinoJson::JsonArray coords = obj["coordinates"];
    lon = coords[0];
    lat = coords[1];
  }
};
