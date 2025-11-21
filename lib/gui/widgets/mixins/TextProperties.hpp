#pragma once

#include "../../types/Color.hpp"
#include "../../types/StyleSelector.hpp"
#include "misc/lv_text.h"

namespace LVGL_Wrapper {

template <typename Derived>
class TextProperties {
 public:
  Derived& set_style_text_color(const Color& color,
                                StyleSelector selector = {}) {
    lv_obj_t* obj = static_cast<Derived*>(this)->raw();
    if (obj)
      lv_obj_set_style_text_color(obj, color, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& set_style_text_font(const lv_font_t* font,
                               StyleSelector selector = {}) {
    lv_obj_t* obj = static_cast<Derived*>(this)->raw();
    if (obj)
      lv_obj_set_style_text_font(obj, font, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& set_style_text_align(const lv_text_align_t align,
                                StyleSelector selector = {}) {
    lv_obj_t* obj = static_cast<Derived*>(this)->raw();
    if (obj)
      lv_obj_set_style_text_align(obj, align, selector);

    return *static_cast<Derived*>(this);
  }
};

}  // namespace LVGL_Wrapper
