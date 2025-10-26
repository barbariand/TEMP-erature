#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

/**
 * @brief A heap-less singleton wrapper for the active screen.
 */
class Screen final : public Widget {
 private:
  // Private constructor for singleton
  Screen() : Widget(nullptr) {}

  // Static instance (no heap)
  static Screen s_instance;
  static bool s_is_initialized;

 public:
  // Delete copy/move
  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  /**
   * @brief Gets the singleton instance of the Screen.
   * *MUST* be called for the first time *after* lv_init().
   */
  static Screen& getInstance() {
    if (!s_is_initialized) {
      // This is the "second stage" of init for this singleton
      s_instance.wrap(lv_scr_act());
      s_is_initialized = true;
    }
    return s_instance;
  }
};
}  // namespace LVGL_Wrapper
