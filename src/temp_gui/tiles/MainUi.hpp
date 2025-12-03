#pragma once
#include <GUI.hpp>

class MainUi : public LVGL_Wrapper::Component {
 private:
  std::shared_ptr<LVGL_Wrapper::Label> m_label;

 public:
  explicit MainUi(LVGL_Wrapper::BaseWidget* parent);
  void init() override;
};
