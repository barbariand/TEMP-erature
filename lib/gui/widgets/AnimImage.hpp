#pragma once
#include "./Image.hpp"
#include <vector>

namespace LVGL_Wrapper {

class AnimImage : public Image {
  private:
    std::vector<const void*> m_anim_srcs;

    explicit AnimImage(BaseWidget* parent)
        : Image(lv_animimg_create, parent) {}

  public:
    static std::shared_ptr<AnimImage> create(BaseWidget& parent) {
        auto new_animimg = std::shared_ptr<AnimImage>(new AnimImage(&parent));
        parent.register_child(new_animimg);
        return new_animimg;
    }

    AnimImage& start() {
        if (m_obj)
            lv_animimg_start(m_obj);
        return *this;
    }

    AnimImage& set_src(const lv_image_dsc_t* dsc[], uint8_t num) {
        if (m_obj) {
            m_anim_srcs.assign(
                reinterpret_cast<const void* const*>(dsc),
                reinterpret_cast<const void* const*>(dsc) + num);
            lv_animimg_set_src(m_obj, m_anim_srcs.data(), num);
        }
        return *this;
    }

    AnimImage& set_duration(uint32_t duration_ms) {
        if (m_obj)
            lv_animimg_set_duration(m_obj, duration_ms);
        return *this;
    }

    AnimImage& set_repeat_count(uint16_t count) {
        if (m_obj)
            lv_animimg_set_repeat_count(m_obj, count);
        return *this;
    }
};

} // namespace LVGL_Wrapper
