#include "DataPoint.hpp"
#include "Date.hpp"
void DataPoint::fromJson(JsonObject obj) {
  date = SimpleDate::getLocalTimeFromUTCStamp(obj["date"].as<int>());
  value = obj["value"].as<float>();
}
