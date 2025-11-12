#pragma once
#include <functional>
#include <vector>
#include "../types/Color.hpp"
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Canvas : public Widget {
 private:
  std::vector<uint8_t> m_buffer_storage;

  explicit Canvas(BaseWidget* parent) : Widget(lv_canvas_create, parent) {}

 public:
  static std::shared_ptr<Canvas> create(BaseWidget& parent) {
    auto new_canvas = std::shared_ptr<Canvas>(new Canvas(&parent));
    parent.register_child(new_canvas);
    return new_canvas;
  }

  Canvas& set_buffer(uint32_t w, uint32_t h, lv_color_format_t cf) {
    if (!m_obj)
      return *this;

    uint32_t stride = lv_draw_buf_width_to_stride(w, cf);
    uint32_t buf_size_bytes = stride * h;

    m_buffer_storage.resize(buf_size_bytes);

    lv_canvas_set_buffer(m_obj, m_buffer_storage.data(), w, h, cf);

    return *this;
  }

  Canvas& set_palette(uint8_t id, const Color& color) {
    if (m_obj)

      lv_canvas_set_palette(m_obj, id, color);
    return *this;
  }

  Canvas& set_px(int32_t x, int32_t y, const Color& color,
                 lv_opa_t opa = LV_OPA_COVER) {
    if (m_obj)

      lv_canvas_set_px(m_obj, x, y, color, opa);
    return *this;
  }

  Color get_px(int32_t x, int32_t y) {
    if (!m_obj)
      return Color();

    lv_color32_t c32 = lv_canvas_get_px(m_obj, x, y);
    return Color::from_lv_color32(c32);
  }

  Canvas& fill_bg(const Color& color, lv_opa_t opa = LV_OPA_COVER) {
    if (m_obj)

      lv_canvas_fill_bg(m_obj, color, opa);
    return *this;
  }
  Canvas& draw(std::function<void(lv_layer_t* layer)> draw_cb) {
    if (!m_obj)
      return *this;

    lv_layer_t layer;
    lv_canvas_init_layer(m_obj, &layer);
    draw_cb(&layer);

    lv_canvas_finish_layer(m_obj, &layer);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
