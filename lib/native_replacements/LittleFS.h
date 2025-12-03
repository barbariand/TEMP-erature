
#pragma once
#if !defined(ARDUINO_ARCH_ESP32)
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include "Arduino.h"
#define FILE_READ 1
#define FILE_WRITE 2
class LittleFSReplacement;
class File : public std::fstream {
  friend LittleFSReplacement;
  File(std::string path) : std::fstream(path) {}

 public:
  bool operator!() { return true; }
  size_t size() {
    seekg(0, std::ios::end);
    return tellg();
  }
  bool close() {
    ((std::fstream*)this)->close();
    return true;
  }
  void readBytes(char* buff, int size) { read(buff, size); }
};
class LittleFSReplacement {
 public:
  bool begin(bool t) { return t; };
  bool exists(String s) {

    std::ifstream f(s);
    return f.is_open();
  }
  File open(String path, int code) { return File(path); }
};

static LittleFSReplacement LittleFS;
#endif
