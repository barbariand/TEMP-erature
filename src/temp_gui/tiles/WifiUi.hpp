#pragma once
#include <GUI.hpp>

class WifiUi : public LVGL_Wrapper::Component {
 private:
  std::shared_ptr<LVGL_Wrapper::Label> m_label;
  bool m_is_dark;

  void update_style();

 public:
  explicit WifiUi(LVGL_Wrapper::BaseWidget* parent);
  void init() override;
};
