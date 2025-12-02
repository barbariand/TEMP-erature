#pragma once
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
struct SimpleDate {
  int year, month, day, hour, minute;
  SimpleDate() {};
  SimpleDate(const std::tm& t) {
    year = t.tm_year + 1900;
    month = t.tm_mon + 1;
    day = t.tm_mday;
    hour = t.tm_hour;
    minute = t.tm_min;
  }

 public:
  SimpleDate static getLocalTimeFromIso(const std::string& str);
  SimpleDate static getLocalTimeFromUTCStamp(const int);
  std::string toString(const std::string& str) const;

};

time_t parseIsoTime(const std::string& isoString);
