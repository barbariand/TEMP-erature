#pragma once

#ifndef ARDUINO
#include <SDL3/SDL.h>
#include "IDisplay.h"

namespace hal {
class SDLDisplay : public IDisplay {
 public:
  SDLDisplay();
  ~SDLDisplay();
  bool init() override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;

  // Helper for the main loop
  SDL_Renderer* getRenderer();

 private:
  int screen_width;
  int screen_height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
};
}  // namespace hal
#endif
