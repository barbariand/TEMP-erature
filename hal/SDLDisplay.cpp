#include <SDL3/SDL_render.h>  // Make sure this is included
#ifndef ARDUINO
#include <iostream>  // For error logging if SDL_Log is not preferred
#include "SDLDisplay.h"

// --- Define the static buffer(s) outside the class ---
lv_color_t hal::SDLDisplay::buf1[800 * hal::SDLDisplay::BUF_LINES];
lv_color_t hal::SDLDisplay::buf2[800 * hal::SDLDisplay::BUF_LINES];

namespace hal {

SDLDisplay::SDLDisplay()
    : screen_width(800),
      screen_height(600),
      window(nullptr),
      renderer(nullptr),
      texture(nullptr) {
  lv_disp_draw_buf_init(&disp_buf, buf1,
                        buf2 /* or nullptr if single buffered */,
                        screen_width * BUF_LINES);
  lv_disp_drv_init(&disp_drv);
}

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

  // --- SDL Initialization ---
  // --- CORRECTED CHECK FOR BOOL RETURN ---
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  // --- Log SDL Version AFTER successful init ---
  SDL_Log("Using SDL Version: %d", SDL_GetVersion());

  window =
      SDL_CreateWindow("LVGL SDL3 Simulation", screen_width, screen_height, 0);
  if (!window) {
    SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  // --- Restore default renderer selection ---
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  } else {
    const char* renderer_name = SDL_GetRendererName(renderer);
    SDL_Log("--- Using Renderer: %s ---",
            renderer_name ? renderer_name : "Unknown");
  }

  // --- Restore STREAMING access mode ---
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

  // --- TEST CALL with CORRECTED CHECK ---
  float test_w, test_h;
  SDL_ClearError();
  // --- CORRECTED CHECK FOR BOOL RETURN ---
  bool test_result = SDL_GetTextureSize(texture, &test_w, &test_h);
  if (!test_result) {  // Check for false
    const char* sdl_error = SDL_GetError();
    SDL_Log("!!! TEST SDL_GetTextureSize in init() FAILED! Error: %s",
            sdl_error ? sdl_error : "None");
  } else {
    SDL_Log("--- TEST SDL_GetTextureSize in init() SUCCEEDED: w=%.0f, h=%.0f",
            test_w, test_h);
  }
  // --- END OF TEST CALL ---

  std::cout << "SDL Initialized Successfully." << std::endl;

  lv_init();
  std::cout << "LVGL Initialized." << std::endl;

  disp_drv.flush_cb = flush_cb_static;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.hor_res = screen_width;
  disp_drv.ver_res = screen_height;
  disp_drv.user_data = this;
  SDL_Log("Setting user_data in driver to: %p", (void*)this);

  lv_disp_t* disp = lv_disp_drv_register(&disp_drv);
  if (!disp) {
    SDL_Log("LVGL display driver registration failed!");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  std::cout << "LVGL Display Driver Registered Successfully." << std::endl;
  std::cout << "SDLDisplay Initialization Complete." << std::endl;
  SDL_Log("Init running on thread ID: %lu", SDL_GetCurrentThreadID());

  // Create a default group
  lv_group_t* group = lv_group_create();
  lv_group_set_default(group);

  // Register SDL mouse input device
  lv_indev_t* mouse_indev = lv_sdl_mouse_create();
  if (mouse_indev) {
    lv_indev_set_group(mouse_indev, group);
    lv_indev_set_disp(mouse_indev, disp);  // Use lv_indev_set_disp in v9+

    // Optional: Add a cursor object (needs LV_USE_IMAGE enabled in lv_conf.h)
    // LV_IMAGE_DECLARE(mouse_cursor_icon); // Needs the mouse icon C array
    // lv_obj_t * cursor_obj = lv_image_create(lv_screen_active());
    // lv_image_set_src(cursor_obj, &mouse_cursor_icon);
    // lv_indev_set_cursor(mouse_indev, cursor_obj);
  } else {
    std::cerr << "Error: Failed to create SDL mouse input device!" << std::endl;
  }

  // Optional: Register SDL mouse wheel input device
  lv_indev_t* mousewheel_indev = lv_sdl_mousewheel_create();
  if (mousewheel_indev) {
    lv_indev_set_group(mousewheel_indev, group);
    lv_indev_set_disp(mousewheel_indev, disp);  // Use lv_indev_set_disp in v9+
  } else {
    std::cerr << "Warning: Failed to create SDL mousewheel input device."
              << std::endl;
  }

  // Optional: Register SDL keyboard input device
  lv_indev_t* keyboard_indev = lv_sdl_keyboard_create();
  if (keyboard_indev) {
    lv_indev_set_group(keyboard_indev, group);
    lv_indev_set_disp(keyboard_indev, disp);  // Use lv_indev_set_disp in v9+
  } else {
    std::cerr << "Warning: Failed to create SDL keyboard input device."
              << std::endl;
  }
  // --- End LVGL Input Device Initialization ---
  return true;
}

// --- Static Callback Bridge ---
void SDLDisplay::flush_cb_static(lv_disp_drv_t* drv, const lv_area_t* area,
                                 lv_color_t* color_p) {
  // SDL_Log("flush_cb_static called. drv->user_data: %p", drv->user_data);
  SDLDisplay* display = static_cast<SDLDisplay*>(drv->user_data);
  if (display) {
    // SDL_Log("Casted user_data OK. Calling flush_display...");
    display->flush_display(drv, area, color_p);
  } else {
    SDL_Log("Error: user_data not found in flush_cb_static!");
    lv_disp_flush_ready(drv);
  }
}

// --- Member Flush Function ---
void SDLDisplay::flush_display(lv_disp_drv_t* drv, const lv_area_t* area,
                               lv_color_t* color_p) {
  // SDL_Log("Flush running on thread ID: %lu", SDL_GetCurrentThreadID());

  if (!renderer || !texture || !drv || !area || !color_p) {
    SDL_Log(
        "Error: NULL pointer passed to flush_display (renderer=%p, "
        "texture=%p)!",
        (void*)renderer, (void*)texture);
    if (drv)
      lv_disp_flush_ready(drv);
    return;
  }

  SDL_ClearError();

  float tex_w_float, tex_h_float;
  // --- CORRECTED CHECK FOR BOOL RETURN ---
  bool query_result = SDL_GetTextureSize(texture, &tex_w_float, &tex_h_float);
  if (!query_result) {  // Check for false
    const char* sdl_error = SDL_GetError();
    SDL_Log("SDL_GetTextureSize failed! Error: %s",
            sdl_error ? sdl_error : "None");
    lv_disp_flush_ready(drv);
    return;
  }

  int tex_w = static_cast<int>(tex_w_float);
  int tex_h = static_cast<int>(tex_h_float);

  int x1 = (int)area->x1;
  int y1 = (int)area->y1;
  int x2 = (int)area->x2;
  int y2 = (int)area->y2;

  if (x1 >= tex_w || y1 >= tex_h || x2 < 0 || y2 < 0) {
    lv_disp_flush_ready(drv);
    return;
  }

  x1 = (x1 < 0) ? 0 : x1;
  y1 = (y1 < 0) ? 0 : y1;
  x2 = (x2 >= tex_w) ? tex_w - 1 : x2;
  y2 = (y2 >= tex_h) ? tex_h - 1 : y2;

  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;

  if (w <= 0 || h <= 0) {
    lv_disp_flush_ready(drv);
    return;
  }

  SDL_Rect r = {x1, y1, w, h};
  int source_pitch = drv->hor_res * sizeof(lv_color_t);

  // --- CORRECTED CHECK FOR BOOL RETURN ---
  if (!SDL_UpdateTexture(texture, &r, color_p, source_pitch)) {
    SDL_Log("SDL_UpdateTexture failed: %s", SDL_GetError());
    // Might still try to render what's there? Or return? Let's try returning.
    lv_disp_flush_ready(drv);
    return;
  }

  // --- CORRECTED CHECK FOR BOOL RETURN ---
  if (!SDL_RenderTexture(renderer, texture, nullptr, nullptr)) {
    SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    lv_disp_flush_ready(drv);
    return;
  }

  // --- CORRECTED CHECK FOR BOOL RETURN ---
  if (!SDL_RenderPresent(renderer)) {
    SDL_Log("SDL_RenderPresent failed: %s", SDL_GetError());
  }

  lv_disp_flush_ready(drv);
}

// --- Other Methods ---
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
    if (event.type == SDL_EVENT_QUIT) {
      return 1;
    }
  }
  return 0;
}

}  // namespace hal
#endif  // ARDUINO
