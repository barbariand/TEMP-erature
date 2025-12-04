#pragma once
#include <string>
#include "api/parameters/MeterologyCode.hpp"

struct UserPrefs {
  std::string chosenPlace;   // Namn för UI
  int chosenId;              // Station-ID
  double chosenLat;
  double chosenLon;
  MeterologyCode chosenMetric; // vald parameter
};
