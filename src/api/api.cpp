#include "api.hpp"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <map>
#include <string>
#include <iostream>

// City coordinates mapping (latitude, longitude)
static const std::map<std::string, std::pair<float, float>> CITY_COORDS = {
    {"Karlskrona", {56.161, 15.586}},
    {"Stockholm", {59.329, 18.068}},
    {"Gothenburg", {57.707, 11.968}},
    {"Malmo", {55.605, 13.003}},
    {"Uppsala", {59.861, 17.640}}
};

// Forward declare the static JSON from Temp_gui.cpp
extern const char* JSON;

ForecastSevenDay fetch_forecast_for_city(const char* city) {
  ForecastSevenDay empty_data;
  
  if (!city || strlen(city) == 0) {
    std::cout << "fetch_forecast_for_city: city is empty" << std::endl;
    return empty_data;
  }

  // Look up coordinates for the city
  auto it = CITY_COORDS.find(std::string(city));
  if (it == CITY_COORDS.end()) {
    std::cout << "fetch_forecast_for_city: City not found in mapping: " << city << std::endl;
    // Fallback to static JSON
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError error =
        ArduinoJson::deserializeJson(doc, JSON);
    if (!error) {
      ForecastSevenDay data;
      data.fromJson(doc);
      return data;
    }
    return empty_data;
  }

  float lat = it->second.first;
  float lon = it->second.second;

  // Build SMHI API URL
  char url[256];
  snprintf(url, sizeof(url),
           "https://opendata-download-metfcst.smhi.se/api/category/pmp3g/version/2/geotype/point/lon/%.3f/lat/%.3f/data.json",
           lon, lat);

  std::cout << "Attempting to fetch forecast for " << city << " from API..." << std::endl;

  HTTPClient http;
  http.setTimeout(15000);
  http.setConnectTimeout(10000);
  
  if (!http.begin(url)) {
    std::cout << "Failed to begin HTTP connection, using fallback JSON" << std::endl;
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError error =
        ArduinoJson::deserializeJson(doc, JSON);
    if (!error) {
      ForecastSevenDay data;
      data.fromJson(doc);
      return data;
    }
    return empty_data;
  }

  int httpCode = http.GET();
  std::cout << "HTTP Response Code: " << httpCode << std::endl;

  if (httpCode != 200) {
    std::cout << "HTTP Error, using fallback JSON" << std::endl;
    http.end();
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError error =
        ArduinoJson::deserializeJson(doc, JSON);
    if (!error) {
      ForecastSevenDay data;
      data.fromJson(doc);
      return data;
    }
    return empty_data;
  }

  // Attempt to parse from stream, but fallback to static JSON on any error
  WiFiClient* stream = http.getStreamPtr();
  
  if (!stream) {
    std::cout << "Failed to get response stream, using fallback JSON" << std::endl;
    http.end();
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError error =
        ArduinoJson::deserializeJson(doc, JSON);
    if (!error) {
      ForecastSevenDay data;
      data.fromJson(doc);
      return data;
    }
    return empty_data;
  }

  // Try to parse API response
  ArduinoJson::JsonDocument doc;
  ArduinoJson::DeserializationError error =
      ArduinoJson::deserializeJson(doc, *stream);

  http.end();

  if (error) {
    std::cout << "API JSON parse failed (code: " << (int)error.code() 
              << "), using fallback JSON" << std::endl;
    // Always fallback to static JSON when API parsing fails
    ArduinoJson::JsonDocument fallback_doc;
    ArduinoJson::DeserializationError fallback_error =
        ArduinoJson::deserializeJson(fallback_doc, JSON);
    if (!fallback_error) {
      ForecastSevenDay data;
      data.fromJson(fallback_doc);
      std::cout << "Successfully loaded fallback JSON for " << city << std::endl;
      return data;
    }
    return empty_data;
  }

  // API parsing succeeded - try to extract data
  ForecastSevenDay data;
  try {
    data.fromJson(doc);
    std::cout << "Successfully fetched and parsed forecast data for " << city << std::endl;
    return data;
  } catch (...) {
    std::cout << "Exception parsing API data, using fallback JSON" << std::endl;
    // Fallback to static JSON on any exception
    ArduinoJson::JsonDocument fallback_doc;
    ArduinoJson::DeserializationError fallback_error =
        ArduinoJson::deserializeJson(fallback_doc, JSON);
    if (!fallback_error) {
      ForecastSevenDay data2;
      data2.fromJson(fallback_doc);
      return data2;
    }
    return empty_data;
  }
}
