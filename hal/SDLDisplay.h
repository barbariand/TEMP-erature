#ifndef ARDUINO
#pragma once  // Use include guards

#include <SDL3/SDL.h>
#include <lvgl.h>
#include "IDisplay.h"  // Assuming IDisplay is your interface

namespace hal {

class SDLDisplay : public IDisplay {
 public:
  SDLDisplay();
  ~SDLDisplay() override;

  bool init() override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;
  int handle_events() override;

  // Keep getRenderer if needed elsewhere, otherwise maybe remove
  SDL_Renderer* getRenderer();

 private:
  // Static callback function to bridge C-style LVGL callback and C++ member function
  static void flush_cb_static(lv_disp_drv_t* drv, const lv_area_t* area,
                              lv_color_t* color_p);

  // Member function to handle the actual flushing
  void flush_display(lv_disp_drv_t* drv, const lv_area_t* area,
                     lv_color_t* color_p);

  int screen_width;
  int screen_height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;

  // LVGL specific members
  lv_disp_draw_buf_t disp_buf;
  // Define buffer size appropriately (e.g., 1/10th of screen height)
  static constexpr int BUF_LINES = 60;      // Example: 60 lines
  static lv_color_t buf1[800 * BUF_LINES];  // Adjust size based on screen_width
  static lv_color_t buf2[800 * BUF_LINES];  // Optional second buffer

  lv_disp_drv_t disp_drv;
};

}  // namespace hal
#endif  // ARDUINO
