#ifndef ARDUINO_ARCH_ESP32
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
  SDL_Log("--- SANITY CHECK ---");
  SDL_Log("sizeof(lv_color_t) = %d bytes", (int)sizeof(lv_color_t));
  if (sizeof(lv_color_t) != 3) {
    SDL_Log(
        "CRITICAL: sizeof(lv_color_t) is NOT 3! Check LV_COLOR_DEPTH in "
        "lv_conf.h!");
  }
  SDL_Log("--------------------");

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
  const Uint32 sdl_pixel_format = SDL_PIXELFORMAT_BGR24;
  texture =
      SDL_CreateTexture(renderer, sdl_pixel_format, SDL_TEXTUREACCESS_STREAMING,
                        screen_width, screen_height);
  SDL_Log("Attempting to create texture with format: BGR24 (%s)",
          SDL_GetPixelFormatName(sdl_pixel_format));

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
  SDL_Log("Buffer size set to: %u bytes (%d w * %d h * %d bpp)",
          (unsigned int)buf_size_bytes, SCREEN_WIDTH, SCREEN_HEIGHT,
          (int)sizeof(lv_color_t));

  lv_display_set_flush_cb(disp_drv, flush_cb_static);

  lv_display_set_buffers(disp_drv, buf1, buf2, buf_size_bytes,
                         LV_DISPLAY_RENDER_MODE_DIRECT);

  lv_display_set_color_format(disp_drv, LV_COLOR_FORMAT_RGB888);
  SDL_Log("Setting render mode to: LV_DISPLAY_RENDER_MODE_DIRECT");

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

  SDL_Log("--- FLUSH (DIRECT MODE - LOCKTEXTURE) ---");

  if (!renderer || !texture || !drv || !area || !color_p) {
    SDL_Log("Error: NULL pointer passed to flush_display!");
    if (drv)
      lv_display_flush_ready(drv);
    return;
  }

  int x1 = (int)area->x1;
  int y1 = (int)area->y1;
  int x2 = (int)area->x2;
  int y2 = (int)area->y2;
  int w = lv_area_get_width(area);
  int h = lv_area_get_height(area);

  SDL_Log("Area to flush: x1=%d, y1=%d, w=%d, h=%d", x1, y1, w, h);

  if (w <= 0 || h <= 0) {
    SDL_Log("Skipping flush: w or h is <= 0.");
    lv_display_flush_ready(drv);
    return;
  }

  void* texture_pixels = nullptr;
  int texture_pitch = 0;
  SDL_Rect r = {x1, y1, w, h};
  if (SDL_LockTexture(texture, &r, &texture_pixels, &texture_pitch) != 0) {
    SDL_Log("SDL_LockTexture failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  uint8_t* source_buffer = color_p;
  int source_pitch = screen_width * sizeof(lv_color_t);

  uint8_t* source_start =
      source_buffer + (y1 * source_pitch) + (x1 * sizeof(lv_color_t));

  uint8_t* dest_pixels = static_cast<uint8_t*>(texture_pixels);

  SDL_Log("Texture locked: dest_ptr=%p, texture_pitch=%d", (void*)dest_pixels,
          texture_pitch);
  SDL_Log("Source locked:  source_start=%p, source_pitch=%d",
          (void*)source_start, source_pitch);

  int bytes_per_row_to_copy = w * sizeof(lv_color_t);

  if (texture_pitch == source_pitch) {
    SDL_Log("Pitches match! Doing single memcpy.");
    int total_bytes = bytes_per_row_to_copy * h;
    memcpy(dest_pixels, source_start, total_bytes);
  } else {
    SDL_Log("Pitch mismatch! Copying row-by-row.");
    for (int y = 0; y < h; ++y) {
      memcpy(dest_pixels + (y * texture_pitch),
             source_start + (y * source_pitch), bytes_per_row_to_copy);
    }
  }

  SDL_UnlockTexture(texture);
  if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
    SDL_Log("SDL_RenderCopy failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  bool is_last = lv_display_flush_is_last(drv);
  SDL_Log("Is last flush? %s", is_last ? "YES" : "NO");

  if (is_last) {
    SDL_Log(">>> Presenting to screen <<<");
    SDL_RenderPresent(renderer);
  }
  SDL_Log("--- FLUSH END ---");

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
      return 1;  // Signal to exit
    }
  }
  return 0;  // Continue running
}

}  // namespace hal
#endif
