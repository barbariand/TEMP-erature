#pragma once
#include <string>
struct MeterologyCodeInfo {
  std::string name;
  std::string unit;
};
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
  static constexpr Type AirTemperature_Momentary =
      Type::AirTemperature_Momentary;
  static constexpr Type AirTemperature_DailyMean =
      Type::AirTemperature_DailyMean;
  static constexpr Type WindDirection = Type::WindDirection;
  static constexpr Type WindSpeed = Type::WindSpeed;
  static constexpr Type PrecipitationAmount_DailySum =
      Type::PrecipitationAmount_DailySum;
  static constexpr Type Relative_Humidity = Type::Relative_Humidity;
  static constexpr Type PrecipitationAmount_HourlySum =
      Type::PrecipitationAmount_HourlySum;
  static constexpr Type SnowDepth = Type::SnowDepth;
  static constexpr Type AirPressure_Reduced = Type::AirPressure_Reduced;
  static constexpr Type SunshineDuration = Type::SunshineDuration;
  static constexpr Type Global_Irradiance = Type::Global_Irradiance;
  static constexpr Type Visibility = Type::Visibility;
  static constexpr Type Current_Weather = Type::Current_Weather;
  static constexpr Type PrecipitationAmount_15minSum =
      Type::PrecipitationAmount_15minSum;
  static constexpr Type PrecipitationIntensity_Max15min =
      Type::PrecipitationIntensity_Max15min;
  static constexpr Type Total_CloudCover = Type::Total_CloudCover;
  static constexpr Type Precipitation_TwiceDaily =
      Type::Precipitation_TwiceDaily;
  static constexpr Type Precipitation_OnceDaily = Type::Precipitation_OnceDaily;
  static constexpr Type AirTemperature_DailyMin = Type::AirTemperature_DailyMin;
  static constexpr Type AirTemperature_DailyMax = Type::AirTemperature_DailyMax;
  static constexpr Type WindGust = Type::WindGust;
  static constexpr Type AirTemperature_MonthlyMean =
      Type::AirTemperature_MonthlyMean;
  static constexpr Type PrecipitationAmount_MonthlySum =
      Type::PrecipitationAmount_MonthlySum;
  static constexpr Type Longwave_Irradiance = Type::Longwave_Irradiance;
  static constexpr Type Max_Of_MeanWindSpeed = Type::Max_Of_MeanWindSpeed;
  static constexpr Type AirTemperature_MinTwiceDaily =
      Type::AirTemperature_MinTwiceDaily;
  static constexpr Type AirTemperature_MaxTwiceDaily =
      Type::AirTemperature_MaxTwiceDaily;
  static constexpr Type CloudBase_LowestLayer = Type::CloudBase_LowestLayer;
  static constexpr Type CloudAmount_LowestLayer = Type::CloudAmount_LowestLayer;
  static constexpr Type CloudBase_SecondLayer = Type::CloudBase_SecondLayer;
  static constexpr Type CloudAmount_SecondLayer = Type::CloudAmount_SecondLayer;
  static constexpr Type CloudBase_ThirdLayer = Type::CloudBase_ThirdLayer;
  static constexpr Type CloudAmount_ThirdLayer = Type::CloudAmount_ThirdLayer;
  static constexpr Type CloudBase_FourthLayer = Type::CloudBase_FourthLayer;
  static constexpr Type CloudAmount_FourthLayer = Type::CloudAmount_FourthLayer;
  static constexpr Type CloudBase_LowestCloudBase =
      Type::CloudBase_LowestCloudBase;
  static constexpr Type CloudBase_LowestCloudBaseMin =
      Type::CloudBase_LowestCloudBaseMin;
  static constexpr Type PrecipitationIntensity_MaxMean15min =
      Type::PrecipitationIntensity_MaxMean15min;
  static constexpr Type DewPointTemperature = Type::DewPointTemperature;
  static constexpr Type Ground_Condition = Type::Ground_Condition;
  static constexpr Type Unknown_Parameter = Type::Unknown_Parameter;

  Type value;

 public:
  bool operator!() { return value != Type::Unknown_Parameter; }
  MeterologyCode(Type t = Unknown_Parameter) : value(t) {}
  explicit MeterologyCode(int code);

  bool operator==(const MeterologyCode& other) const {
    return value == other.value;
  }
  bool operator==(Type other) const { return value == other; }
  bool operator!=(const MeterologyCode& other) const {
    return value != other.value;
  }
  bool operator!=(Type other) const { return value != other; }

  operator Type() const { return value; }

  explicit operator int() const { return static_cast<int>(value); }
  MeterologyCodeInfo toInfo() const;
};
