#pragma once
#include <vector>

struct CityInfo {
  const char* name;
  double lon;
  double lat;
};

// Small static list of known cities (name, lon, lat)
static const std::vector<CityInfo> kKnownCities = {
    {"Karlskrona", 15.5869, 56.1612},
    {"Stockholm", 18.0686, 59.3293},
    {"Gothenburg", 11.9746, 57.7089},
    {"Malmö", 13.0038, 55.6050},
    {"Uppsala", 17.6389, 59.8586},
};
