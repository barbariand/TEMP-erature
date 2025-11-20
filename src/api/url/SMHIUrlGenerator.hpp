#include <string>
constexpr const char* BASE_URL = "http://opendata-download-metfcst.smhi.se";
#include "../parameters/SevenDayForcastParameters.hpp"
std::string seven_day_forcast_data_url(SevenDayForcastParameters params);
