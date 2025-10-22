#ifndef ARDUINO
#include "SDLDisplay.h"

namespace hal {
SDLDisplay::SDLDisplay()
    : screen_width(800),
      screen_height(600),
      window(nullptr),
      renderer(nullptr),
      texture(nullptr) {}

SDLDisplay::~SDLDisplay() {
  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

bool SDLDisplay::init() {

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("ESP32-T4-S3 simulation", screen_width,
                            screen_height, 0);
  renderer = SDL_CreateRenderer(window, "main");
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                              SDL_TEXTUREACCESS_STREAMING, screen_width,
                              screen_height);
  lv_init();
  return true;
}

void flush(SDL_Renderer* renderer, SDL_Texture* texture, lv_disp_drv_t* drv,
           const lv_area_t* area, lv_color_t* color_p) {
  SDL_Rect r = {(int)area->x1, (int)area->y1, (int)lv_area_get_width(area),
                (int)lv_area_get_height(area)};
  SDL_UpdateTexture(texture, &r, color_p, drv->hor_res * sizeof(lv_color_t));
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
  lv_disp_flush_ready(drv);
}
void SDLDisplay::setBrightness(uint8_t brightness) {}
void SDLDisplay::sleep() {}
void SDLDisplay::wakeup() {}
SDL_Renderer* SDLDisplay::getRenderer() {
  return renderer;
}
}  // namespace hal
#endif
