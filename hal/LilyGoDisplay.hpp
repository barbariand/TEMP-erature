#pragma once
#include "IDisplay.hpp"
#include "LilyGo/LilyGo_AMOLED.h"
namespace hal {
class LilyGoDisplay : public IDisplay {
 public:
  LilyGoDisplay();
  ~LilyGoDisplay();
  bool init() override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;
  int handle_events() override;

 private:
  LilyGo_AMOLED* amoled;
};
}  // namespace hal
