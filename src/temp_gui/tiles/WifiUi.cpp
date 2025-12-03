#include "WifiUi.hpp"

using namespace LVGL_Wrapper;

WifiUi::WifiUi(BaseWidget* parent) : Component(parent), m_is_dark(false) {}

void WifiUi::init() {
  set_size(LV_PCT(100), LV_PCT(100));
  update_style();

  m_label = Label::create(*this);
  m_label->set_text("Connect to WiFi")
      .set_style_text_font(&lv_font_montserrat_28)
      .center();

  add_flag(Flag::Clickable);

  on_clicked([this]() {
    m_is_dark = !m_is_dark;
    update_style();
  });
}

void WifiUi::update_style() {
  if (m_is_dark) {
    set_style_bg_color(Color::Black);
    if (m_label)
      m_label->set_style_text_color(Color::White);
  } else {
    set_style_bg_color(Color::White);
    if (m_label)
      m_label->set_style_text_color(Color::Black);
  }
  set_style_bg_opa(Opa::Cover);
}
