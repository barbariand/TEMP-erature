#include "MeterologyCode.hpp"

MeterologyCode::MeterologyCode(int code) {

  if (code >= 1 && code <= 40) {
    value = static_cast<Type>(code);
  } else {
    value = Type::Unknown_Parameter;
  }
}

std::string MeterologyCode::toString() const {
  switch (value) {
    case Type::AirTemperature_Momentary:
      return "Lufttemperatur (momentanvärde, 1 gång/tim) [°C]";
    case Type::AirTemperature_DailyMean:
      return "Lufttemperatur (medelvärde 1 dygn) [°C]";
    case Type::WindDirection:
      return "Vindriktning (medelvärde 10 min) [grader]";
    case Type::WindSpeed:
      return "Vindhastighet (medelvärde 10 min) [m/s]";
    case Type::PrecipitationAmount_DailySum:
      return "Nederbördsmängd (summa 1 dygn) [mm]";
    case Type::Relative_Humidity:
      return "Relativ Luftfuktighet (momentanvärde) [%]";
    case Type::PrecipitationAmount_HourlySum:
      return "Nederbördsmängd (summa 1 timme) [mm]";
    case Type::SnowDepth:
      return "Snödjup (momentanvärde) [meter]";
    case Type::AirPressure_Reduced:
      return "Lufttryck reducerat havsytans nivå (momentanvärde) [hPa]";
    case Type::SunshineDuration:
      return "Solskenstid (summa 1 timme) [sekund]";
    case Type::Global_Irradiance:
      return "Global Irradians (medelvärde 1 timma) [W/m²]";
    case Type::Visibility:
      return "Sikt (momentanvärde) [meter]";
    case Type::Current_Weather:
      return "Rådande väder (momentanvärde) [kod]";
    case Type::PrecipitationAmount_15minSum:
      return "Nederbördsmängd (summa 15 min) [mm]";
    case Type::PrecipitationIntensity_Max15min:
      return "Nederbördsintensitet (max under 15 min) [mm/s]";
    case Type::Total_CloudCover:
      return "Total molnmängd (momentanvärde) [%]";
    case Type::Precipitation_TwiceDaily:
      return "Nederbörd (2 gånger/dygn) [kod]";
    case Type::Precipitation_OnceDaily:
      return "Nederbörd (1 gång/dygn) [kod]";
    case Type::AirTemperature_DailyMin:
      return "Lufttemperatur (min, 1 gång per dygn) [°C]";
    case Type::AirTemperature_DailyMax:
      return "Lufttemperatur (max, 1 gång per dygn) [°C]";
    case Type::WindGust:
      return "Byvind (max, 1 gång/tim) [m/s]";
    case Type::AirTemperature_MonthlyMean:
      return "Lufttemperatur (medel, 1 gång per månad) [°C]";
    case Type::PrecipitationAmount_MonthlySum:
      return "Nederbördsmängd (summa, 1 gång per månad) [mm]";
    case Type::Longwave_Irradiance:
      return "Långvågs-Irradians (medelvärde 1 timma) [W/m²]";
    case Type::Max_Of_MeanWindSpeed:
      return "Max av MedelVindhastighet (maximum av medelvärde 10 min) [m/s]";
    case Type::AirTemperature_MinTwiceDaily:
      return "Lufttemperatur (min, 2 gånger per dygn) [°C]";
    case Type::AirTemperature_MaxTwiceDaily:
      return "Lufttemperatur (max, 2 gånger per dygn) [°C]";
    case Type::CloudBase_LowestLayer:
      return "Molnbas (lägsta molnlager) [meter]";
    case Type::CloudAmount_LowestLayer:
      return "Molnmängd (lägsta molnlager) [kod]";
    case Type::CloudBase_SecondLayer:
      return "Molnbas (andra molnlager) [meter]";
    case Type::CloudAmount_SecondLayer:
      return "Molnmängd (andra molnlager) [kod]";
    case Type::CloudBase_ThirdLayer:
      return "Molnbas (tredje molnlager) [meter]";
    case Type::CloudAmount_ThirdLayer:
      return "Molnmängd (tredje molnlager) [kod]";
    case Type::CloudBase_FourthLayer:
      return "Molnbas (fjärde molnlager) [meter]";
    case Type::CloudAmount_FourthLayer:
      return "Molnmängd (fjärde molnlager) [kod]";
    case Type::CloudBase_LowestCloudBase:
      return "Molnbas (lägsta molnbas, momentanvärde) [meter]";
    case Type::CloudBase_LowestCloudBaseMin:
      return "Molnbas (lägsta molnbas, min under 15 min) [meter]";
    case Type::PrecipitationIntensity_MaxMean15min:
      return "Nederbördsintensitet (max av medel under 15 min) [mm/s]";
    case Type::DewPointTemperature:
      return "Daggpunktstemperatur (momentanvärde) [°C]";
    case Type::Ground_Condition:
      return "Markens tillstånd (momentanvärde) [kod]";

    case Type::Unknown_Parameter:
    default:
      return "Okänd meteorologisk parameter";
  }
}
