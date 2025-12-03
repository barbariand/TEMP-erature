#if !defined(ARDUINO_ARCH_ESP32)
#include <string>
#include "Arduino.h"
#define HTTP_CODE_OK 200
class HTTPClient {
 public:
  void setTimeout(int time) {};
  void begin(std::string s) {};
  int GET() {return 200;};
  String getString();
  void end() {  };
};
#endif
