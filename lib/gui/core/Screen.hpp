#pragma once
#include <mutex>
#include "./Widget.hpp"
#include "lvgl.h"

namespace LVGL_Wrapper {

class Screen final : public Widget {
 protected:
  Screen() : Widget(lv_scr_act()) {}

 private:
  static std::shared_ptr<Screen> s_instance;
  static std::once_flag s_instance_flag;

 public:
  ~Screen() override { m_obj = nullptr; }
  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  static std::shared_ptr<Screen> getInstance();
};

}  // namespace LVGL_Wrapper
