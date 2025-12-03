#pragma once
#include <ArduinoJson.h>
#include "api/data/ForcastSevenDays.hpp"
#include "api/data/LatestMonthsStation.hpp"
#include "api/data/ObservationSeries.hpp"
#include "api/parameters/SevenDayForcastParameters.hpp"
#include "api/parameters/StationsHistoricalParameters.hpp"
#include "api/parameters/StationsLatestMonthsParameters.hpp"

// Minimal SMHI client helpers used by the UI.
// Provides best-effort methods to fetch the 7-day forecast or latest-months observation
// data given lon/lat coordinates. These are lightweight and intended for on-device
// usage; they return parsed JSON into the provided ArduinoJson document.

bool fetch_seven_day_forecast(SevenDayForcastParameters params,
                              ForecastSevenDay& out);
bool fetch_stations_latest_months(StationsHistoricalParameters params,
                                  LatestMonthsStations& outDoc);
bool fetch_latest_months(StationsLatestMonthsParameters params,
                         ObservationSeries& outDoc);
