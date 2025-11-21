
#pragma once
#include <iostream>
#include <string>

struct WeatherSymbol {
  // Alla 27 koder enligt SMHI:s SNOW dokumentation
  enum class Type : int {
    ClearSky = 1,
    NearlyClearSky = 2,
    VariableCloudiness = 3,
    HalfclearSky = 4,
    CloudySky = 5,
    Overcast = 6,
    Fog = 7,
    LightRainShowers = 8,
    ModerateRainShowers = 9,
    HeavyRainShowers = 10,
    Thunderstorm = 11,
    LightSleetShowers = 12,
    ModerateSleetShowers = 13,
    HeavySleetShowers = 14,
    LightSnowShowers = 15,
    ModerateSnowShowers = 16,
    HeavySnowShowers = 17,
    LightRain = 18,
    ModerateRain = 19,
    HeavyRain = 20,
    Thunder = 21,
    LightSleet = 22,
    ModerateSleet = 23,
    HeavySleet = 24,
    LightSnowfall = 25,
    ModerateSnowfall = 26,
    HeavySnowfall = 27,
    Unknown = 0
  };

  Type value;

  WeatherSymbol(int code = 0);
  std::string toString() const;
  const char* get_weather_icon_str();
};
