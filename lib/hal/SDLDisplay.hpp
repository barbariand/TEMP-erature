#pragma once
#ifdef SDL_BUILD

#include <SDL2/SDL.h>
#include <lvgl.h>
#include <atomic>
#include "IDisplay.hpp"                   // Your interface

namespace hal {

class SDLDisplay : public IDisplay {
 public:
  SDLDisplay();
  ~SDLDisplay();

  // --- IDisplay Interface Methods ---
  bool init() override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;
  int handle_events() override;
  // ----------------------------------

  SDL_Renderer* getRenderer();  // Kept this from your original

 private:
  // Static callback to safely catch the SDL_QUIT event
  static int quit_event_filter(void* userdata, SDL_Event* event);

  // Define screen dimensions as constants
  static constexpr int SCREEN_WIDTH = 800;
  static constexpr int SCREEN_HEIGHT = 600;

  int screen_width;
  int screen_height;

  lv_display_t* disp_drv;  // We only need to store the display driver

  // Flag to signal exit to the main loop
  static std::atomic<bool> quit_signaled;
};

}  // namespace hal
#endif  // SDL_BUILD
