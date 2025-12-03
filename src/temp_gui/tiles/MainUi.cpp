#include "MainUi.hpp"

using namespace LVGL_Wrapper;

MainUi::MainUi(BaseWidget* parent) : Component(parent) {}

void MainUi::init() {
  set_size(LV_PCT(100), LV_PCT(100));
  set_style_bg_color(Color::White);
  set_style_bg_opa(Opa::Cover);

  m_label = Label::create(*this);
  m_label->set_text("Group 15\nVersion 0.10")
      .set_style_text_font(&lv_font_montserrat_28)
      .set_style_text_align(LV_TEXT_ALIGN_CENTER)
      .center();
}
