#ifndef ARDUINO
#pragma once  // Use include guards

#include <SDL2/SDL.h>
#include <lvgl.h>
#include "IDisplay.hpp"  // Assuming IDisplay is your interface

namespace hal {

class SDLDisplay : public IDisplay {
 public:
  SDLDisplay();
  ~SDLDisplay();

  bool init() override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;
  int handle_events() override;

  // Keep getRenderer if needed elsewhere, otherwise maybe remove
  SDL_Renderer* getRenderer();

 private:
  // Static callback function to bridge C-style LVGL callback and C++ member function
  static void flush_cb_static(lv_display_t* drv, const lv_area_t* area,
                              uint8_t* color_p);

  // Member function to handle the actual flushing
  void flush_display(lv_display_t* drv, const lv_area_t* area,
                     uint8_t* color_p);

  int screen_width;
  int screen_height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;

  lv_draw_buf_t disp_buf;
  static constexpr int BUF_LINES = 600;
  static lv_color_t buf1[800 * BUF_LINES];
  static lv_color_t buf2[800 * BUF_LINES];
  lv_display_t* disp_drv;
};

}  // namespace hal
#endif  // ARDUINO
