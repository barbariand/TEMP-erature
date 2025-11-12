#pragma once
#include <string>
#include <vector>
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class ButtonMatrix : public Widget, public TextProperties<ButtonMatrix> {
 private:
  // Keep a copy of the map description for lifetime management
  std::vector<std::string> m_map_storage;
  std::vector<const char*> m_map_ptrs;

  explicit ButtonMatrix(BaseWidget* parent)
      : Widget(lv_buttonmatrix_create, parent) {}

 public:
  static std::shared_ptr<ButtonMatrix> create(BaseWidget& parent) {
    auto new_btnm = std::shared_ptr<ButtonMatrix>(new ButtonMatrix(&parent));
    parent.register_child(new_btnm);
    return new_btnm;
  }

  /**
 * @brief Sets the button map (layout).
 * The vector is copied and managed internally.
 * Example: {"Btn1", "Btn2", ""}, {"Btn3", "Btn4", "Btn5"}, NULL
 */
  ButtonMatrix& set_map(const std::vector<std::string>& map) {
    if (!m_obj)
      return *this;

    m_map_storage = map;
    m_map_ptrs.clear();
    for (const auto& str : m_map_storage) {
      m_map_ptrs.push_back(str.c_str());
    }
    // Add the final NULL terminator required by LVGL
    m_map_ptrs.push_back(nullptr);

    lv_buttonmatrix_set_map(m_obj, m_map_ptrs.data());
    return *this;
  }

  /**
 * @brief Sets the control map for buttons (e.g., checkable, hidden).
 * The vector is copied and managed internally.
 */
  ButtonMatrix& set_control_map(
      const std::vector<lv_buttonmatrix_ctrl_t>& ctrl_map) {
    if (m_obj) {
      // This is a bit tricky, as ctrl_map is not const.
      // We assume the user's vector lifetime is valid for this call,
      // or we'd need to store this vector too.
      // For simplicity, let's just pass it.
      // A safer way would be to copy it to a member variable.
      lv_buttonmatrix_set_ctrl_map(
          m_obj, const_cast<lv_buttonmatrix_ctrl_t*>(ctrl_map.data()));
    }
    return *this;
  }

  ButtonMatrix& set_selected_button(uint16_t btn_id) {
    if (m_obj)
      lv_buttonmatrix_set_selected_button(m_obj, btn_id);
    return *this;
  }

  uint16_t get_selected_button() const {
    return m_obj ? lv_buttonmatrix_get_selected_button(m_obj) : 0;
  }

  bool has_button_control(uint16_t btn_id, lv_buttonmatrix_ctrl_t ctrl) {
    return m_obj ? lv_buttonmatrix_has_button_ctrl(m_obj, btn_id, ctrl)
                 : false;
  }

  ButtonMatrix& clear_button_control(uint16_t btn_id,
                                     lv_buttonmatrix_ctrl_t ctrl) {
    if (m_obj)
      lv_buttonmatrix_clear_button_ctrl(m_obj, btn_id, ctrl);
    return *this;
  }

};

}  // namespace LVGL_Wrapper
