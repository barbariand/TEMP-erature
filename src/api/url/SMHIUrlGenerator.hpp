#pragma once
#include <string>
#include "api/parameters/SevenDayForcastParameters.hpp"
#include "api/parameters/StationsHistoricalParameters.hpp"
#include "api/parameters/StationsLatestMonthsParameters.hpp"
#include "api/smhi_client.hpp"
std::string seven_day_forcast_data_url(SevenDayForcastParameters params);
std::string stations_historical(StationsHistoricalParameters params);
std::string latest_months_data(StationsLatestMonthsParameters params);
