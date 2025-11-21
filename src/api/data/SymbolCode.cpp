#include "api/data/SymbolCode.hpp"
WeatherSymbol::WeatherSymbol(int code) {
  if (code >= 1 && code <= 27) {
    value = static_cast<Type>(code);
  } else {
    value = Type::Unknown;
  }
};
std::string WeatherSymbol::toString() const {
  switch (value) {
    case Type::ClearSky:
      return "Klart";
    case Type::NearlyClearSky:
      return "Lätt molnighet";
    case Type::VariableCloudiness:
      return "Växlande molnighet";
    case Type::HalfclearSky:
      return "Halvklart";
    case Type::CloudySky:
      return "Molnigt";
    case Type::Overcast:
      return "Mulet";
    case Type::Fog:
      return "Dimma";
    case Type::LightRainShowers:
      return "Lätt regnskur";
    case Type::ModerateRainShowers:
      return "Regnskur";
    case Type::HeavyRainShowers:
      return "Kraftig regnskur";
    case Type::Thunderstorm:
      return "Åskskur";
    case Type::LightSleetShowers:
      return "Lätt by av snöblandat regn";
    case Type::ModerateSleetShowers:
      return "By av snöblandat regn";
    case Type::HeavySleetShowers:
      return "Kraftig by av snöblandat regn";
    case Type::LightSnowShowers:
      return "Lätt snöby";
    case Type::ModerateSnowShowers:
      return "Snöby";
    case Type::HeavySnowShowers:
      return "Kraftig snöby";
    case Type::LightRain:
      return "Lätt regn";
    case Type::ModerateRain:
      return "Regn";
    case Type::HeavyRain:
      return "Kraftigt regn";
    case Type::Thunder:
      return "Åska";
    case Type::LightSleet:
      return "Lätt snöblandat regn";
    case Type::ModerateSleet:
      return "Snöblandat regn";
    case Type::HeavySleet:
      return "Kraftigt snöblandat regn";
    case Type::LightSnowfall:
      return "Lätt snöfall";
    case Type::ModerateSnowfall:
      return "Snöfall";
    case Type::HeavySnowfall:
      return "Kraftigt snöfall";
    default:
      return "Okänt";
  }
}
 const char* WeatherSymbol::get_weather_icon_str() const {
  // Dessa koder är specifika för weather_fontfonten.
  switch ((int)this->value) {
    case 1:
      return "\uf00d";  // Klart (Day Sunny)
    case 2:
      return "\uf00c";  // Lätt molnighet (Day Sunny Overcast)
    case 3:
      return "\uf002";  // Halvklart (Cloudy)
    case 4:
      return "\uf013";  // Molnigt (Cloudy)
    case 5:
      return "\uf013";  // Mycket molnigt
    case 6:
      return "\uf013";  // Mulet
    case 7:
      return "\uf014";  // Dimma (Fog)
    case 8:
      return "\uf009";  // Lätt regnskur (Showers)
    case 9:
      return "\uf019";  // Regnskur
    case 10:
      return "\uf019";  // Kraftig regnskur
    case 11:
      return "\uf01e";  // Åskskur (Thunderstorm)
    case 12:
      return "\uf0b2";  // Lätt by av snöblandat regn (Sleet)
    case 13:
      return "\uf0b5";  // Snöblandat regn
    case 14:
      return "\uf0b5";
    case 15:
      return "\uf00a";  // Lätt snöby (Snow)
    case 16:
      return "\uf01b";  // Snöby
    case 17:
      return "\uf01b";
    case 18:
      return "\uf017";  // Lätt regn (Rain)
    case 19:
      return "\uf019";  // Regn
    case 20:
      return "\uf019";
    case 21:
      return "\uf016";  // Åska (Lightning)
    case 22:
      return "\uf0b2";  // Lätt snöblandat regn
    case 23:
      return "\uf0b5";
    case 24:
      return "\uf0b5";
    case 25:
      return "\uf01b";  // Snöfall (Snow)
    case 26:
      return "\uf01b";
    case 27:
      return "\uf064";  // Kraftigt snöfall
    default:
      return "\uf07b";  // "NA" ikon
  }
}
