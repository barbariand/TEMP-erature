
#pragma once
#include <ArduinoJson.hpp>
#include "SymbolCode.hpp"
using ArduinoJson::JsonObject;
struct ForecastData {
  float airTemperature;
  float windDirection;
  float windSpeed;
  float windGust;
  int relativeHumidity;
  float airPressure;
  float visibility;
  float thunderProb;
  float frozenPrecipProb;

  // Cloud details
  int cloudAreaFraction;
  int lowCloudFraction;
  int mediumCloudFraction;
  int highCloudFraction;
  int cloudBaseAltitude;
  int cloudTopAltitude;

  float precipitationMean;
  float precipitationMin;
  float precipitationMax;
  int precipProb;
  float precipFrozenPart;
  int precipType;

  WeatherSymbol symbolCode;

  void fromJson(JsonObject obj);
};
