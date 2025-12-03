#if !defined(ARDUINO_ARCH_ESP32)
#include <cmath>
#include <string>
class String : public std::string {};
float round(float in){
  return std::round(in);
}
#endif
