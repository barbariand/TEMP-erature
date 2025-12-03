
#pragma once
#if !defined(ARDUINO_ARCH_ESP32)
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <string>

class String : public std::string {
 public:
  String(const char* str) : std::string(str) {};
  String(std::string& str) : std::string(str) {};
  String(float f) : std::string(std::to_string(f)) {};
  String(int f) : std::string(std::to_string(f)) {};
  String() {};
  ~String() {};
  bool equals(String& other) { return *this == other; }
  bool equals(const String& other) { return *this == other; }
  int toInt() { return std::stoi(*this); }
  float toFloat() { return std::stof(*this); }
  friend class HTTPClient;
};
#endif
