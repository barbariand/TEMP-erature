#include "SMHIUrlGenerator.hpp"

#define FORCAST_BASE_URL "http://opendata-download-metfcst.smhi.se"
std::string seven_day_forcast_data_url(SevenDayForcastParameters params) {
  char buffer[256];
  /// 76 data point max usualy means that it is within 7 days or a bit more
  std::snprintf(buffer, sizeof(buffer),
                FORCAST_BASE_URL
                "/api/category/snow1g/version/1/geotype/point/lon/%.4f/lat/"
                "%.4f/data.json?timeseries=76",
                params.location.lon, params.location.lat);
  return std::string(buffer);
}

#define OBSERVATION_BASE_URL "http://opendata-download-metfcst.smhi.se"
std::string stations_historical(StationsHistorical params) {
  if (!params.meterology) {
    return "";
  }
  char buffer[256];
  std::snprintf(buffer, sizeof(buffer),
                OBSERVATION_BASE_URL "api/version/latest/parameter/%d.json",
                (int)params.meterology);
  return std::string(buffer);
}
std::string latest_months_data(StationsHistoricalLatestMonths params) {
  if (!params.meterology) {
    return "";
  }
  char buffer[256];
  std::snprintf(buffer, sizeof(buffer),
                OBSERVATION_BASE_URL
                "/api/version/1.0/parameter/%d/station/%d/period/"
                "latest-months/data.json",
                (int)params.meterology, params.station);
  return std::string(buffer);
}
