#include <gtest/gtest.h>
#include <api/url/SMHIUrlGenerator.hpp>
TEST(URLGENERATOR, GENERALTESTING) {
  // Test a standard coordinate set (e.g., Karlskrona)
  SevenDayForcastParameters params;
  params.location.lon = 15.5869;
  params.location.lat = 56.1612;

  std::string expected =
      "http://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/"
      "geotype/point/lon/15.5869/lat/56.1612/data.json?timeseries=76";
  std::string actual = seven_day_forcast_data_url(params);

  EXPECT_EQ(actual, expected);
}

TEST(URLGENERATOR, CUTOF_LONGITUDE) {
  // Test that Longitude with many decimals is rounded correctly (10.12345 -> 10.1235)
  SevenDayForcastParameters params;
  params.location.lon = 10.12345;
  params.location.lat = 55.0000;

  std::string expected =
      "http://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/"
      "geotype/point/lon/10.1235/lat/55.0000/data.json?timeseries=76";
  std::string actual = seven_day_forcast_data_url(params);

  EXPECT_EQ(actual, expected);
}

TEST(URLGENERATOR, CUTOF_LATITUDE) {
  // Test that Latitude with excessive precision is truncated/rounded (55.98764 -> 55.9876)
  SevenDayForcastParameters params;
  params.location.lon = 10.0000;
  params.location.lat = 55.98764;

  std::string expected =
      "http://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/"
      "geotype/point/lon/10.0000/lat/55.9876/data.json?timeseries=76";
  std::string actual = seven_day_forcast_data_url(params);

  EXPECT_EQ(actual, expected);
}

TEST(URLGENERATOR, CUTOF) {
  // Test the specific rounding behavior of printf's %.4f
  // 18.55555 should truncate to 18.5555
  // 56.44444 should truncate to 56.4444
  SevenDayForcastParameters params;
  params.location.lon = 18.55555;
  params.location.lat = 56.44444;

  std::string expected =
      "http://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/"
      "geotype/point/lon/18.5555/lat/56.4444/data.json?timeseries=76";
  std::string actual = seven_day_forcast_data_url(params);

  EXPECT_EQ(actual, expected);
}
