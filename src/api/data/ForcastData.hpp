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

  void fromJson(JsonObject obj) {
    airTemperature = obj["air_temperature"];
    windDirection = obj["wind_from_direction"];
    windSpeed = obj["wind_speed"];
    windGust = obj["wind_speed_of_gust"];
    relativeHumidity = obj["relative_humidity"];
    airPressure = obj["air_pressure_at_mean_sea_level"];
    visibility = obj["visibility_in_air"];
    thunderProb = obj["thunderstorm_probability"];
    frozenPrecipProb = obj["probability_of_frozen_precipitation"];

    cloudAreaFraction = obj["cloud_area_fraction"];
    lowCloudFraction = obj["low_type_cloud_area_fraction"];
    mediumCloudFraction = obj["medium_type_cloud_area_fraction"];
    highCloudFraction = obj["high_type_cloud_area_fraction"];
    cloudBaseAltitude = obj["cloud_base_altitude"];
    cloudTopAltitude = obj["cloud_top_altitude"];

    precipitationMean = obj["precipitation_amount_mean"];
    precipitationMin = obj["precipitation_amount_min"];
    precipitationMax = obj["precipitation_amount_max"];
    precipProb = obj["probability_of_precipitation"];
    precipFrozenPart = obj["precipitation_frozen_part"];
    precipType = obj["predominant_precipitation_type_at_surface"];

    symbolCode = obj["symbol_code"];
  }
};
