#include "./Screen.hpp"

namespace LVGL_Wrapper {

std::shared_ptr<Screen> Screen::s_instance = nullptr;
std::once_flag Screen::s_instance_flag;

std::shared_ptr<Screen> Screen::getInstance() {
  std::call_once(s_instance_flag,
                 []() { s_instance = std::shared_ptr<Screen>(new Screen()); });
  return s_instance;
}

}  // namespace LVGL_Wrapper
