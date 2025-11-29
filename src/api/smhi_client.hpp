#pragma once
#include <ArduinoJson.h>

// Minimal SMHI client helpers used by the UI.
// Provides best-effort methods to fetch the 7-day forecast or latest-months observation
// data given lon/lat coordinates. These are lightweight and intended for on-device
// usage; they return parsed JSON into the provided ArduinoJson document.

bool fetch_seven_day_forecast(double lon, double lat, ArduinoJson::DynamicJsonDocument& outDoc, unsigned long timeout_ms = 10000);
bool fetch_latest_months(double lon, double lat, ArduinoJson::DynamicJsonDocument& outDoc, unsigned long timeout_ms = 15000);
