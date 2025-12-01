#include "MeterologyParameters.hpp"

MeterologyParameter::MeterologyParameter(int code) {

  if (code >= 1 && code <= 40) {
    value = static_cast<Type>(code);
  } else {
    value = Type::Okänd_Parameter;
  }
}

std::string MeterologyParameter::toString() const {
  switch (value) {
    case Type::Lufttemperatur_Momentan:
      return "Lufttemperatur (momentanvärde, 1 gång/tim) [°C]";
    case Type::Lufttemperatur_MedelDygn:
      return "Lufttemperatur (medelvärde 1 dygn) [°C]";
    case Type::Vindriktning:
      return "Vindriktning (medelvärde 10 min) [grader]";
    case Type::Vindhastighet:
      return "Vindhastighet (medelvärde 10 min) [m/s]";
    case Type::Nederbördsmängd_SummaDygn:
      return "Nederbördsmängd (summa 1 dygn) [mm]";
    case Type::Relativ_Luftfuktighet:
      return "Relativ Luftfuktighet (momentanvärde) [%]";
    case Type::Nederbördsmängd_SummaTimme:
      return "Nederbördsmängd (summa 1 timme) [mm]";
    case Type::Snödjup:
      return "Snödjup (momentanvärde) [meter]";
    case Type::Lufttryck_Reducerat:
      return "Lufttryck reducerat havsytans nivå (momentanvärde) [hPa]";
    case Type::Solskenstid:
      return "Solskenstid (summa 1 timme) [sekund]";
    case Type::Global_Irradians:
      return "Global Irradians (medelvärde 1 timma) [W/m²]";
    case Type::Sikt:
      return "Sikt (momentanvärde) [meter]";
    case Type::Rådande_Väder:
      return "Rådande väder (momentanvärde) [kod]";
    case Type::Nederbördsmängd_Summa15min:
      return "Nederbördsmängd (summa 15 min) [mm]";
    case Type::Nederbördsintensitet_Max15min:
      return "Nederbördsintensitet (max under 15 min) [mm/s]";
    case Type::Total_Molnmängd:
      return "Total molnmängd (momentanvärde) [%]";
    case Type::Nederbörd_TvåGångerDygn:
      return "Nederbörd (2 gånger/dygn) [kod]";
    case Type::Nederbörd_EnGångDygn:
      return "Nederbörd (1 gång/dygn) [kod]";
    case Type::Lufttemperatur_MinDygn:
      return "Lufttemperatur (min, 1 gång per dygn) [°C]";
    case Type::Lufttemperatur_MaxDygn:
      return "Lufttemperatur (max, 1 gång per dygn) [°C]";
    case Type::Byvind:
      return "Byvind (max, 1 gång/tim) [m/s]";
    case Type::Lufttemperatur_MedelMånad:
      return "Lufttemperatur (medel, 1 gång per månad) [°C]";
    case Type::Nederbördsmängd_SummaMånad:
      return "Nederbördsmängd (summa, 1 gång per månad) [mm]";
    case Type::Långvågs_Irradians:
      return "Långvågs-Irradians (medelvärde 1 timma) [W/m²]";
    case Type::Max_Av_MedelVindhastighet:
      return "Max av MedelVindhastighet (maximum av medelvärde 10 min) [m/s]";
    case Type::Lufttemperatur_MinTvåGångerDygn:
      return "Lufttemperatur (min, 2 gånger per dygn) [°C]";
    case Type::Lufttemperatur_MaxTvåGångerDygn:
      return "Lufttemperatur (max, 2 gånger per dygn) [°C]";
    case Type::Molnbas_LägstaLager:
      return "Molnbas (lägsta molnlager) [meter]";
    case Type::Molnmängd_LägstaLager:
      return "Molnmängd (lägsta molnlager) [kod]";
    case Type::Molnbas_AndraLager:
      return "Molnbas (andra molnlager) [meter]";
    case Type::Molnmängd_AndraLager:
      return "Molnmängd (andra molnlager) [kod]";
    case Type::Molnbas_TredjeLager:
      return "Molnbas (tredje molnlager) [meter]";
    case Type::Molnmängd_TredjeLager:
      return "Molnmängd (tredje molnlager) [kod]";
    case Type::Molnbas_FjärdeLager:
      return "Molnbas (fjärde molnlager) [meter]";
    case Type::Molnmängd_FjärdeLager:
      return "Molnmängd (fjärde molnlager) [kod]";
    case Type::Molnbas_LägstaMolnbas:
      return "Molnbas (lägsta molnbas, momentanvärde) [meter]";
    case Type::Molnbas_LägstaMolnbasMin:
      return "Molnbas (lägsta molnbas, min under 15 min) [meter]";
    case Type::Nederbördsintensitet_MaxMedel15min:
      return "Nederbördsintensitet (max av medel under 15 min) [mm/s]";
    case Type::Daggpunktstemperatur:
      return "Daggpunktstemperatur (momentanvärde) [°C]";
    case Type::Markens_Tillstånd:
      return "Markens tillstånd (momentanvärde) [kod]";

    case Type::Okänd_Parameter:
    default:
      return "Okänd meteorologisk parameter";
  }
}
