#pragma once
#include <vector>

struct CityInfo {
  const char* name;
  double lon;
  double lat;
  int station;
};

// Small static list of known cities (name, lon, lat)
static const std::vector<CityInfo> kKnownCities = {
    {"Karlskrona", 15.5869, 56.1612, 65090},
    {"Stockholm", 18.0686, 59.3293, 97400},
    {"Göteborg", 11.9746, 57.7089, 72420},
    {"Malmö", 13.0038, 55.6050, 53300},
    {"Kiruna", 20.24915, 67.85857, 180940},
};
