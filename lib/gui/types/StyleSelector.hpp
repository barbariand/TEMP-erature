#pragma once
#include "./Enums.hpp"
#include "lvgl.h"

namespace LVGL_Wrapper {

class StyleSelector {
  lv_style_selector_t m_selector;

 public:
  StyleSelector(Part::Type part = Part::Main,
                State::Type state = State::Default)
      : m_selector(part | state) {}

  StyleSelector& part(Part::Type p) {
    m_selector = (m_selector & ~LV_PART_ANY) | p;
    return *this;
  }

  StyleSelector& state(State::Type s) {
    m_selector = (m_selector & ~LV_STATE_ANY) | s;
    return *this;
  }

  StyleSelector& add_state(State::Type s) {
    m_selector |= s;
    return *this;
  }

  constexpr operator lv_style_selector_t() const { return m_selector; }
};
}  // namespace LVGL_Wrapper
