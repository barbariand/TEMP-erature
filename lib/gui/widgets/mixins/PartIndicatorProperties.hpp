#pragma once

#include "../../types/StyleSelector.hpp"
#include "../../types/Enums.hpp"
#include "../../types/Color.hpp"

namespace LVGL_Wrapper {

template <typename Derived>
class PartIndicatorProperties {
 protected:
  lv_obj_t* self() { return static_cast<Derived*>(this)->raw(); }

  Derived& derived() { return *static_cast<Derived*>(this); }

 public:
  Derived& set_style_bg_color_indicator(const Color& color,
                                        StyleSelector selector = {}) {
    if (self())
      lv_obj_set_style_bg_color(self(), color,
                                selector.part(Part::Indicator));
    return derived();
  }

  Derived& set_style_bg_opa_indicator(lv_opa_t opa,
                                      StyleSelector selector = {}) {
    if (self())
      lv_obj_set_style_bg_opa(self(), opa, selector.part(Part::Indicator));
    return derived();
  }

  Derived& set_style_radius_indicator(int32_t radius,
                                      StyleSelector selector = {}) {
    if (self())
      lv_obj_set_style_radius(self(), radius, selector.part(Part::Indicator));
    return derived();
  }

  Derived& set_style_border_width_indicator(int32_t width,
                                            StyleSelector selector = {}) {
    if (self())
      lv_obj_set_style_border_width(self(), width,
                                    selector.part(Part::Indicator));
    return derived();
  }

  Derived& set_style_border_color_indicator(const Color& color,
                                            StyleSelector selector = {}) {
    if (self())
      lv_obj_set_style_border_color(self(), color,
                                    selector.part(Part::Indicator));
    return derived();
  }
};

}  // namespace LVGL_Wrapper
