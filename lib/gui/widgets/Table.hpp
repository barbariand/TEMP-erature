#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Table : public Widget {
 private:
  explicit Table(BaseWidget* parent) : Widget(lv_table_create, parent) {}

 public:
  static std::shared_ptr<Table> create(BaseWidget& parent) {
    auto new_table = std::shared_ptr<Table>(new Table(&parent));
    parent.register_child(new_table);
    return new_table;
  }

  Table& set_cell_value(uint16_t row, uint16_t col, const char* txt) {
    if (m_obj)
      lv_table_set_cell_value(m_obj, row, col, txt);
    return *this;
  }

  Table& set_row_count(uint16_t row_cnt) {
    if (m_obj)
      lv_table_set_row_count(m_obj, row_cnt);
    return *this;
  }

  Table& set_col_count(uint16_t col_cnt) {
    if (m_obj)
      lv_table_set_column_count(m_obj, col_cnt);
    return *this;
  }

  Table& set_col_width(uint16_t col, int32_t w) {
    if (m_obj)
      lv_table_set_column_width(m_obj, col, w);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
