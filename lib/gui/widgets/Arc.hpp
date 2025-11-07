#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Arc : public Widget, public PartIndicatorProperties<Arc> {
  private:
    explicit Arc(BaseWidget* parent) : Widget(lv_arc_create, parent) {}

  public:
    static std::shared_ptr<Arc> create(BaseWidget& parent) {
        auto new_arc = std::shared_ptr<Arc>(new Arc(&parent));
        parent.register_child(new_arc);
        return new_arc;
    }

    Arc& set_range(int16_t min, int16_t max) {
        if (m_obj)
            lv_arc_set_range(m_obj, min, max);
        return *this;
    }

    Arc& set_value(int16_t value) {
        if (m_obj)
            lv_arc_set_value(m_obj, value);
        return *this;
    }

    int16_t get_value() const { return m_obj ? lv_arc_get_value(m_obj) : 0; }

    Arc& set_angles(uint16_t start, uint16_t end) {
        if (m_obj)
            lv_arc_set_angles(m_obj, start, end);
        return *this;
    }

    Arc& set_bg_angles(uint16_t start, uint16_t end) {
        if (m_obj)
            lv_arc_set_bg_angles(m_obj, start, end);
        return *this;
    }
};

} // namespace LVGL_Wrapper
