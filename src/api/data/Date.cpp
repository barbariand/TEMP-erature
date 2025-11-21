#include "Date.hpp"
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>
SimpleDate SimpleDate::getLocalTimeFromIso(const std::string& isoString) {
  std::tm tm_utc = {};
  std::istringstream ss(isoString);
  ss >> std::get_time(&tm_utc, "%Y-%m-%dT%H:%M:%SZ");
  std::time_t tmp = mktime(&tm_utc);
  std::tm* tm_local = localtime_r(&tmp, &tm_utc);

  return SimpleDate(*tm_local);
}

std::string SimpleDate::toString(const std::string& formatStr) const {
  std::tm t = {};
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_isdst = -1;
  std::mktime(&t);
  char buffer[64];
  std::strftime(buffer, sizeof(buffer), formatStr.c_str(), &t);

  return std::string(buffer);
}
