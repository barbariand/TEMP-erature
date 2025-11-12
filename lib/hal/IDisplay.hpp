#pragma once
#include <lvgl.h>

// Interface for any display hardware

namespace hal {
class IDisplay {
 public:
  virtual ~IDisplay() = default;
  virtual bool init() = 0;
  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void sleep() = 0;
  virtual void wakeup() = 0;
  virtual int handle_events() = 0;
};
}  // namespace hal
