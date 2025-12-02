#pragma once
#include <iostream>

struct MeterologyCode {
  enum class Type : int {

    // Lufttemperatur_Momentan
    AirTemperature_Momentary = 1,

    // Lufttemperatur_MedelDygn
    AirTemperature_DailyMean = 2,

    // Vindriktning
    WindDirection = 3,

    // Vindhastighet
    WindSpeed = 4,

    // Nederbördsmängd_SummaDygn
    PrecipitationAmount_DailySum = 5,

    // Relativ_Luftfuktighet
    Relative_Humidity = 6,

    // Nederbördsmängd_SummaTimme
    PrecipitationAmount_HourlySum = 7,

    // Snödjup
    SnowDepth = 8,

    // Lufttryck_Reducerat
    AirPressure_Reduced = 9,

    // Solskenstid
    SunshineDuration = 10,

    // Global_Irradians
    Global_Irradiance = 11,

    // Sikt
    Visibility = 12,

    // Rådande_Väder
    Current_Weather = 13,

    // Nederbördsmängd_Summa15min
    PrecipitationAmount_15minSum = 14,

    // Nederbördsintensitet_Max15min
    PrecipitationIntensity_Max15min = 15,

    // Total_Molnmängd
    Total_CloudCover = 16,

    // Nederbörd_TvåGångerDygn
    Precipitation_TwiceDaily = 17,

    // Nederbörd_EnGångDygn
    Precipitation_OnceDaily = 18,

    // Lufttemperatur_MinDygn
    AirTemperature_DailyMin = 19,

    // Lufttemperatur_MaxDygn
    AirTemperature_DailyMax = 20,

    // Byvind
    WindGust = 21,

    // Lufttemperatur_MedelMånad
    AirTemperature_MonthlyMean = 22,

    // Nederbördsmängd_SummaMånad
    PrecipitationAmount_MonthlySum = 23,

    // Långvågs_Irradians
    Longwave_Irradiance = 24,

    // Max_Av_MedelVindhastighet
    Max_Of_MeanWindSpeed = 25,

    // Lufttemperatur_MinTvåGångerDygn
    AirTemperature_MinTwiceDaily = 26,

    // Lufttemperatur_MaxTvåGångerDygn
    AirTemperature_MaxTwiceDaily = 27,

    // Molnbas_LägstaLager
    CloudBase_LowestLayer = 28,

    // Molnmängd_LägstaLager
    CloudAmount_LowestLayer = 29,

    // Molnbas_AndraLager
    CloudBase_SecondLayer = 30,

    // Molnmängd_AndraLager
    CloudAmount_SecondLayer = 31,

    // Molnbas_TredjeLager
    CloudBase_ThirdLayer = 32,

    // Molnmängd_TredjeLager
    CloudAmount_ThirdLayer = 33,

    // Molnbas_FjärdeLager
    CloudBase_FourthLayer = 34,

    // Molnmängd_FjärdeLager
    CloudAmount_FourthLayer = 35,

    // Molnbas_LägstaMolnbas
    CloudBase_LowestCloudBase = 36,

    // Molnbas_LägstaMolnbasMin
    CloudBase_LowestCloudBaseMin = 37,

    // Nederbördsintensitet_MaxMedel15min
    PrecipitationIntensity_MaxMean15min = 38,

    // Daggpunktstemperatur
    DewPointTemperature = 39,

    // Markens_Tillstånd
    Ground_Condition = 40,

    // Okänd_Parameter
    Unknown_Parameter = 0,
  };

  Type value;

 public:
  MeterologyCode(int code = 0);
  operator int() const { return static_cast<int>(*this); }
  std::string toString() const;
};
