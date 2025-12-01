#pragma once
#include <iostream>

struct MeterologyParameter {
  enum class Type : int {

    Lufttemperatur_Momentan = 1,
    Lufttemperatur_MedelDygn = 2,
    Vindriktning = 3,
    Vindhastighet = 4,
    Nederbördsmängd_SummaDygn = 5,
    Relativ_Luftfuktighet = 6,
    Nederbördsmängd_SummaTimme = 7,
    Snödjup = 8,
    Lufttryck_Reducerat = 9,
    Solskenstid = 10,
    Global_Irradians = 11,
    Sikt = 12,
    Rådande_Väder = 13,
    Nederbördsmängd_Summa15min = 14,
    Nederbördsintensitet_Max15min = 15,
    Total_Molnmängd = 16,
    Nederbörd_TvåGångerDygn = 17,
    Nederbörd_EnGångDygn = 18,
    Lufttemperatur_MinDygn = 19,
    Lufttemperatur_MaxDygn = 20,
    Byvind = 21,
    Lufttemperatur_MedelMånad = 22,
    Nederbördsmängd_SummaMånad = 23,
    Långvågs_Irradians = 24,
    Max_Av_MedelVindhastighet = 25,
    Lufttemperatur_MinTvåGångerDygn = 26,
    Lufttemperatur_MaxTvåGångerDygn = 27,
    Molnbas_LägstaLager = 28,
    Molnmängd_LägstaLager = 29,
    Molnbas_AndraLager = 30,
    Molnmängd_AndraLager = 31,
    Molnbas_TredjeLager = 32,
    Molnmängd_TredjeLager = 33,
    Molnbas_FjärdeLager = 34,
    Molnmängd_FjärdeLager = 35,
    Molnbas_LägstaMolnbas = 36,
    Molnbas_LägstaMolnbasMin = 37,
    Nederbördsintensitet_MaxMedel15min = 38,
    Daggpunktstemperatur = 39,
    Markens_Tillstånd = 40,
    Okänd_Parameter = 0,
  };

  Type value;

 public:
  MeterologyParameter(int code = 0);
  std::string toString() const;
};
