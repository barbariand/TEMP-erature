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
