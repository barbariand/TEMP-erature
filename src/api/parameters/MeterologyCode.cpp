#include "MeterologyCode.hpp"

MeterologyCode::MeterologyCode(int code) {

  if (code >= 1 && code <= 40) {
    value = static_cast<Type>(code);
  } else {
    value = Type::Unknown_Parameter;
  }
}

MeterologyCodeInfo MeterologyCode::toInfo() const {
  switch (value) {
    case Type::AirTemperature_Momentary:
      return {"Lufttemperatur (momentan)", "°C"};
    case Type::AirTemperature_DailyMean:
      return {"Lufttemperatur (dygnsmedel)", "°C"};
    case Type::WindDirection:
      return {"Vindriktning", "°"};
    case Type::WindSpeed:
      return {"Vindhastighet", "m/s"};
    case Type::PrecipitationAmount_DailySum:
      return {"Nederbörd (dygn)", "mm"};
    case Type::Relative_Humidity:
      return {"Luftfuktighet", "%"};
    case Type::PrecipitationAmount_HourlySum:
      return {"Nederbörd (timme)", "mm"};
    case Type::SnowDepth:
      return {"Snödjup", "m"};
    case Type::AirPressure_Reduced:
      return {"Lufttryck", "hPa"};
    case Type::SunshineDuration:
      return {"Solskenstid", "s"};
    case Type::Global_Irradiance:
      return {"Global Irradians", "W/m²"};
    case Type::Visibility:
      return {"Sikt", "m"};
    case Type::Current_Weather:
      return {"Rådande väder", "kod"};
    case Type::PrecipitationAmount_15minSum:
      return {"Nederbörd (15 min)", "mm"};
    case Type::PrecipitationIntensity_Max15min:
      return {"Nederbördsintensitet", "mm/s"};
    case Type::Total_CloudCover:
      return {"Molnmängd", "%"};
    case Type::Precipitation_TwiceDaily:
      return {"Nederbörd (12h)", "kod"};
    case Type::Precipitation_OnceDaily:
      return {"Nederbörd (24h)", "kod"};
    case Type::AirTemperature_DailyMin:
      return {"Lufttemperatur (min)", "°C"};
    case Type::AirTemperature_DailyMax:
      return {"Lufttemperatur (max)", "°C"};
    case Type::WindGust:
      return {"Byvind", "m/s"};
    case Type::AirTemperature_MonthlyMean:
      return {"Lufttemperatur (månad)", "°C"};
    case Type::PrecipitationAmount_MonthlySum:
      return {"Nederbörd (månad)", "mm"};
    case Type::Longwave_Irradiance:
      return {"Långvågs-Irradians", "W/m²"};
    case Type::Max_Of_MeanWindSpeed:
      return {"Max Medelvind", "m/s"};
    case Type::AirTemperature_MinTwiceDaily:
      return {"Lufttemperatur (min 12h)", "°C"};
    case Type::AirTemperature_MaxTwiceDaily:
      return {"Lufttemperatur (max 12h)", "°C"};
    case Type::CloudBase_LowestLayer:
      return {"Molnbas (låg)", "m"};
    case Type::CloudAmount_LowestLayer:
      return {"Molnmängd (låg)", "kod"};
    case Type::CloudBase_SecondLayer:
      return {"Molnbas (mellan)", "m"};
    case Type::CloudAmount_SecondLayer:
      return {"Molnmängd (mellan)", "kod"};
    case Type::CloudBase_ThirdLayer:
      return {"Molnbas (hög)", "m"};
    case Type::CloudAmount_ThirdLayer:
      return {"Molnmängd (hög)", "kod"};
    case Type::CloudBase_FourthLayer:
      return {"Molnbas (extra)", "m"};
    case Type::CloudAmount_FourthLayer:
      return {"Molnmängd (extra)", "kod"};
    case Type::CloudBase_LowestCloudBase:
      return {"Lägsta molnbas", "m"};
    case Type::CloudBase_LowestCloudBaseMin:
      return {"Lägsta molnbas (min)", "m"};
    case Type::PrecipitationIntensity_MaxMean15min:
      return {"Intensitet (medel 15m)", "mm/s"};
    case Type::DewPointTemperature:
      return {"Daggpunkt", "°C"};
    case Type::Ground_Condition:
      return {"Marktillstånd", "kod"};

    case Type::Unknown_Parameter:
    default:
      return {"Okänd parameter", ""};
  }
}
