
#ifdef SDL_BUILD
#include "drivers/sdl/lv_sdl_keyboard.h"
#include "drivers/sdl/lv_sdl_window.h"
#include <SDL2/SDL_version.h>
#include <iostream>
#include "SDLDisplay.hpp"

// Initialize the static quit flag
std::atomic<bool> hal::SDLDisplay::quit_signaled(false);

/**
 * @brief A static SDL event filter.
 * This function is registered with SDL to watch all events.
 * It "steals" the SDL_QUIT event to prevent LVGL's default
 * handler from calling lv_deinit() abruptly.
 */
int hal::SDLDisplay::quit_event_filter(void* userdata, SDL_Event* event) {
  if (event->type == SDL_QUIT) {
    quit_signaled = true;
    return 0;  // Stop the event from being processed further
  }
  return 1;  // Process the event normally
}

namespace hal {

SDLDisplay::SDLDisplay()
    : screen_width(SCREEN_WIDTH),
      screen_height(SCREEN_HEIGHT),
      disp_drv(nullptr) {}

SDLDisplay::~SDLDisplay() {
  lv_deinit();  // LVGL will clean up the window, renderer, etc.
  std::cout << "SDLDisplay destroyed." << std::endl;
}

bool SDLDisplay::init() {
  std::cout << "Initializing LVGL..." << std::endl;
  lv_init();
  std::cout << "LVGL Initialized." << std::endl;

  // This one function creates the SDL window, renderer, display,
  // and all input drivers (mouse, keyboard, etc.)
  disp_drv = lv_sdl_window_create(screen_width, screen_height);

  lv_sdl_mouse_create();
  lv_sdl_keyboard_create();
  if (!disp_drv) {
    std::cerr << "lv_sdl_window_create failed!" << std::endl;
    SDL_Quit();
    return false;
  }

  // Add our custom event filter to catch SDL_QUIT
  SDL_AddEventWatch(quit_event_filter, this);

  // Set up a group for keyboard/mouse navigation
  lv_group_t* group = lv_group_create();
  lv_group_set_default(group);

  // Find the auto-created input devices and add them to the group
  lv_indev_t* indev = lv_indev_get_next(NULL);
  while (indev) {
    if (lv_indev_get_type(indev) == LV_INDEV_TYPE_KEYPAD ||
        lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER) {
      lv_indev_set_group(indev, group);
    }
    indev = lv_indev_get_next(indev);
  }

  std::cout << "LVGL SDL Driver Setup Complete." << std::endl;
  return true;
}

void SDLDisplay::setBrightness(uint8_t brightness) {
  // This is a "best effort" implementation.
  // It may not work on all platforms (e.g., Wayland).
  SDL_Window* window = lv_sdl_window_get_window(disp_drv);
  if (window) {
    float brightness_float = static_cast<float>(brightness) / 255.0f;
    SDL_SetWindowBrightness(window, brightness_float);
  }
}

void SDLDisplay::sleep() {
  SDL_Window* window = lv_sdl_window_get_window(disp_drv);
  if (window) {
    SDL_HideWindow(window);
  }
}

void SDLDisplay::wakeup() {
  SDL_Window* window = lv_sdl_window_get_window(disp_drv);
  if (window) {
    SDL_ShowWindow(window);
  }
}

SDL_Renderer* SDLDisplay::getRenderer() {
  return static_cast<SDL_Renderer*>(lv_sdl_window_get_renderer(disp_drv));
}

int SDLDisplay::handle_events() {
  // This function now does the "real" work by calling lv_timer_handler.
  // lv_timer_handler() internally calls the SDL event poller
  // that was set up by lv_sdl_window_create.
  lv_timer_handler();

  // Check if our quit filter caught an exit request
  if (quit_signaled) {
    return 1;  // Signal to exit
  }

  return 0;  // Continue running
}

}  // namespace hal
#endif  // SDL_BUILD
