#pragma once
#include <algorithm>
#include <vector>
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Line : public Widget {
 private:
  std::vector<lv_point_precise_t> m_points;

  explicit Line(BaseWidget* parent) : Widget(lv_line_create, parent) {}

  void update_points() {
    if (m_obj) {
      lv_line_set_points(m_obj, m_points.data(), (uint32_t)m_points.size());
    }
  }

 public:
  static std::shared_ptr<Line> create(BaseWidget& parent) {
    auto new_line = std::shared_ptr<Line>(new Line(&parent));
    parent.register_child(new_line);
    return new_line;
  }

  Line& set_points(const lv_point_precise_t points[], uint16_t point_num) {
    m_points.assign(points, points + point_num);
    update_points();
    return *this;
  }

  Line& set_points(const std::vector<lv_point_precise_t>& points) {
    m_points = points;
    update_points();
    return *this;
  }

  Line& add_point(lv_value_precise_t x, lv_value_precise_t y) {
    m_points.push_back(lv_point_precise_t{x, y});
    update_points();
    return *this;
  }
  Line& add_point(lv_point_precise_t point) {
    m_points.push_back(point);
    update_points();
    return *this;
  }

  Line& clear_points() {
    m_points.clear();
    update_points();
    return *this;
  }

  bool remove_point_at(uint16_t index) {
    if (index >= m_points.size()) {
      return false;
    }
    m_points.erase(m_points.begin() + index);
    update_points();
    return true;
  }

  const std::vector<lv_point_precise_t>& get_points() const { return m_points; }
};

}  // namespace LVGL_Wrapper
