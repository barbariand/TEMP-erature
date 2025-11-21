#include "ForcastData.hpp"
#include <ArduinoJson.hpp>
using ArduinoJson::JsonObject;
void ForecastData::fromJson(JsonObject obj) {
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
  int temp_code = obj["symbol_code"];
  symbolCode = WeatherSymbol(temp_code);
}
