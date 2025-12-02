#include <string>
#include "../parameters/SevenDayForcastParameters.hpp"
#include "../parameters/StationsHistorical.hpp"
#include "../parameters/StationsLatestMonths.hpp"
std::string seven_day_forcast_data_url(SevenDayForcastParameters params);
std::string stations_historical(StationsHistorical params);
std::string latest_months_data(StationsHistoricalLatestMonths params);
