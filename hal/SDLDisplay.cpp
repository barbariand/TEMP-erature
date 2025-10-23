#ifndef ARDUINO
#include "SDLDisplay.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>
#include <iostream>

lv_color_t hal::SDLDisplay::buf1[hal::SDLDisplay::SCREEN_WIDTH *
                                 hal::SDLDisplay::SCREEN_HEIGHT];
lv_color_t hal::SDLDisplay::buf2[hal::SDLDisplay::SCREEN_WIDTH *
                                 hal::SDLDisplay::SCREEN_HEIGHT];

namespace hal {

SDLDisplay::SDLDisplay()
    : screen_width(SCREEN_WIDTH),
      screen_height(SCREEN_HEIGHT),
      window(nullptr),
      renderer(nullptr),
      texture(nullptr),
      disp_drv(nullptr) {}

SDLDisplay::~SDLDisplay() {

  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "SDLDisplay destroyed." << std::endl;
}

bool SDLDisplay::init() {
  std::cout << "Initializing SDLDisplay..." << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  SDL_version ver;
  SDL_VERSION(&ver);
  SDL_Log("Using SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);

  window = SDL_CreateWindow("LVGL SDL2 Simulation", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
                            SDL_WINDOW_SHOWN);
  if (!window) {
    SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  } else {

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) == 0) {
      SDL_Log("--- Using Renderer: %s ---", info.name ? info.name : "Unknown");
    }
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                              SDL_TEXTUREACCESS_STREAMING, screen_width,
                              screen_height);

  SDL_Log("SDL_CreateTexture result: %p", (void*)texture);
  if (!texture) {
    SDL_Log("Texture could not be created! SDL_Error: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  std::cout << "Initializing LVGL..." << std::endl;
  lv_init();
  std::cout << "LVGL Initialized." << std::endl;

  disp_drv = lv_display_create(screen_width, screen_height);
  SDL_Log("--- DISPLAY INIT ---");
  SDL_Log("lv_display_create called with: width=%d, height=%d", screen_width,
          screen_height);
  SDL_Log("----------------------");
  if (!disp_drv) {
    SDL_Log("LVGL display create failed!");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }
  std::cout << "LVGL Display Driver Created." << std::endl;

  uint32_t buf_size_bytes = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(lv_color_t);

  lv_display_set_flush_cb(disp_drv, flush_cb_static);

  lv_display_set_buffers(disp_drv, buf1, buf2, buf_size_bytes,
                         LV_DISPLAY_RENDER_MODE_FULL);

  lv_display_set_user_data(disp_drv, this);
  SDL_Log("Setting user_data in driver to: %p", (void*)this);

  lv_disp_t* disp = disp_drv;

  std::cout << "LVGL Display Driver Setup Complete." << std::endl;
  SDL_Log("Init running on thread ID: %lu", SDL_ThreadID());

  lv_group_t* group = lv_group_create();
  lv_group_set_default(group);

  lv_indev_t* mouse_indev = lv_sdl_mouse_create();
  if (mouse_indev) {
    lv_indev_set_group(mouse_indev, group);
    lv_indev_set_disp(mouse_indev, disp);
  } else {
    std::cerr << "Error: Failed to create SDL mouse input device!" << std::endl;
  }

  lv_indev_t* mousewheel_indev = lv_sdl_mousewheel_create();
  if (mousewheel_indev) {
    lv_indev_set_group(mousewheel_indev, group);
    lv_indev_set_disp(mousewheel_indev, disp);
  } else {
    std::cerr << "Warning: Failed to create SDL mousewheel input device."
              << std::endl;
  }

  lv_indev_t* keyboard_indev = lv_sdl_keyboard_create();
  if (keyboard_indev) {
    lv_indev_set_group(keyboard_indev, group);
    lv_indev_set_disp(keyboard_indev, disp);
  } else {
    std::cerr << "Warning: Failed to create SDL keyboard input device."
              << std::endl;
  }

  return true;
}

void SDLDisplay::flush_cb_static(lv_display_t* drv, const lv_area_t* area,
                                 uint8_t* color_p) {

  SDLDisplay* display = static_cast<SDLDisplay*>(lv_display_get_user_data(drv));
  if (display) {
    display->flush_display(drv, area, color_p);
  } else {
    SDL_Log("Error: user_data not found in flush_cb_static!");
    lv_display_flush_ready(drv);
  }
}

void SDLDisplay::flush_display(lv_display_t* drv, const lv_area_t* area,
                               uint8_t* color_p) {

  if (!renderer || !texture || !drv || !area || !color_p) {
    SDL_Log(
        "Error: NULL pointer passed to flush_display (renderer=%p, "
        "texture=%p)!",
        (void*)renderer, (void*)texture);
    if (drv)
      lv_display_flush_ready(drv);
    return;
  }

  SDL_ClearError();

  int tex_w, tex_h;
  int query_result = SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
  if (query_result != 0) {
    const char* sdl_error = SDL_GetError();
    SDL_Log("SDL_QueryTexture failed! Error: %s",
            sdl_error ? sdl_error : "None");
    lv_display_flush_ready(drv);
    return;
  }

  int x1 = (int)area->x1;
  int y1 = (int)area->y1;
  int x2 = (int)area->x2;
  int y2 = (int)area->y2;

  if (x1 >= tex_w || y1 >= tex_h || x2 < 0 || y2 < 0) {
    lv_display_flush_ready(drv);
    return;
  }

  x1 = (x1 < 0) ? 0 : x1;
  y1 = (y1 < 0) ? 0 : y1;
  x2 = (x2 >= tex_w) ? tex_w - 1 : x2;
  y2 = (y2 >= tex_h) ? tex_h - 1 : y2;

  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;

  if (w <= 0 || h <= 0) {
    lv_display_flush_ready(drv);
    return;
  }

  SDL_Rect r = {x1, y1, w, h};

  int source_pitch = screen_width * sizeof(lv_color_t);

  uint8_t* source_pixels =
      color_p + (y1 * source_pitch) + (x1 * sizeof(lv_color_t));
  SDL_Log("--- FLUSH ---");
  SDL_Log("Area to flush: x1=%d, y1=%d (w=%d, h=%d)", x1, y1, w, h);
  SDL_Log("Member screen_width: %d", screen_width);
  SDL_Log("Calculated source_pitch: %d bytes (should be 800 * 2 = 1600)",
          source_pitch);
  SDL_Log("Pointers: base=%p, offset_start=%p", (void*)color_p,
          (void*)source_pixels);
  SDL_Log("-------------");
  if (SDL_UpdateTexture(texture, &r, (const void*)source_pixels,
                        source_pitch) != 0) {
    SDL_Log("SDL_UpdateTexture failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
    SDL_Log("SDL_RenderCopy failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  SDL_RenderPresent(renderer);

  lv_display_flush_ready(drv);
}

void SDLDisplay::setBrightness(uint8_t brightness) {
  (void)brightness;
}
void SDLDisplay::sleep() { /* SDL_HideWindow(window); */ }
void SDLDisplay::wakeup() { /* SDL_ShowWindow(window); */ }
SDL_Renderer* SDLDisplay::getRenderer() {
  return renderer;
}
int SDLDisplay::handle_events() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return 1;
    }
  }
  return 0;
}

}  // namespace hal
#endif
