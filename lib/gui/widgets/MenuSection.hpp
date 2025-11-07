#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class MenuPage;  // Forward declaration

/**
 * @brief Represents a "section" in a MenuPage (e.g., a title).
 */
class MenuSection : public Widget {
  friend class MenuPage;

 protected:
  // Constructor for wrapping an existing LVGL object
  explicit MenuSection(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
  // Sections are typically just stylized containers (labels)
};

}  // namespace LVGL_Wrapper
