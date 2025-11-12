#pragma once
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class Roller : public Widget, public TextProperties<Roller> {
  private:
    explicit Roller(BaseWidget* parent) : Widget(lv_roller_create, parent) {}

  public:
    static std::shared_ptr<Roller> create(BaseWidget& parent) {
        auto new_roller = std::shared_ptr<Roller>(new Roller(&parent));
        parent.register_child(new_roller);
        return new_roller;
    }

    Roller& set_options(const char* options, lv_roller_mode_t mode) {
        if (m_obj)
            lv_roller_set_options(m_obj, options, mode);
        return *this;
    }

    Roller& set_selected(uint16_t sel_opt, lv_anim_enable_t anim) {
        if (m_obj)
            lv_roller_set_selected(m_obj, sel_opt, anim);
        return *this;
    }

    uint16_t get_selected() const {
        return m_obj ? lv_roller_get_selected(m_obj) : 0;
    }

    Roller& set_visible_row_count(uint8_t row_cnt) {
        if (m_obj)
            lv_roller_set_visible_row_count(m_obj, row_cnt);
        return *this;
    }
};

} // namespace LVGL_Wrapper
