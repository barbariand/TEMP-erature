#include "SettingsUi.hpp"
#include <iostream>

using namespace LVGL_Wrapper;

SettingsUi::SettingsUi(BaseWidget* parent) : Component(parent) {}

void SettingsUi::load_values_to_ui() {
  m_current_settings = SettingsStorage::load();

  int selected_cityx = 0;
  for (size_t i = 0; i < kKnownCities.size(); ++i) {
    if (kKnownCities[i].station == m_current_settings.city) {
      selected_cityx = i;
      break;
    }
  }
  if (m_city_dd)
    m_city_dd->set_selected(selected_cityx);

  int selected_param_idx = 0;
  for (size_t i = 0; i < m_params.size(); ++i) {
    if (m_params[i].value == m_current_settings.parameter.value) {
      selected_param_idx = i;
      break;
    }
  }
  if (m_param_dd)
    m_param_dd->set_selected(selected_param_idx);
}

void SettingsUi::dispatch_if_changed() {

  SettingsData new_target;

  uint16_t c_idx = m_city_dd->get_selected();
  if (c_idx < kKnownCities.size()) {
    new_target.city = kKnownCities[c_idx].station;
  }

  uint16_t p_idx = m_param_dd->get_selected();
  if (p_idx < m_params.size()) {
    new_target.parameter = m_params[p_idx];
  }

  bool changed = (new_target.city != m_last_applied.city) ||
                 (new_target.parameter.value != m_last_applied.parameter.value);

  if (changed) {
    std::cout << "[SettingsUi] Change detected. Fetching new data..."
              << std::endl;

    m_last_applied = new_target;

    if (on_save) {
      on_save(new_target);
    }
  }
}

void SettingsUi::init() {
  set_size(LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(m_obj, 20, 0);
  lv_obj_set_style_pad_gap(m_obj, 15, 0);

  m_title = Label::create(*this);
  m_title->set_text("Settings")
      .set_style_text_font(&lv_font_montserrat_28)
      .center();

  m_city_label = Label::create(*this);
  m_city_label->set_text("Select City:");

  m_city_names.clear();
  std::string city_opts_str;
  for (const auto& city : kKnownCities) {
    m_city_names.push_back(city.name);
    city_opts_str += std::string(city.name) + "\n";
  }
  if (!city_opts_str.empty())
    city_opts_str.pop_back();

  m_city_dd = Dropdown::create(*this);
  m_city_dd->set_options(city_opts_str.c_str()).set_width(LV_PCT(100));

  m_city_dd->on_value_changed([this]() { dispatch_if_changed(); });

  m_param_label = Label::create(*this);
  m_param_label->set_text("Graph Parameter:");

  m_params = {MeterologyCode::AirTemperature_Momentary,
              MeterologyCode::Relative_Humidity, MeterologyCode::WindSpeed,
              MeterologyCode::AirPressure_Reduced};

  std::string param_opts_str;
  for (const auto& p : m_params) {
    param_opts_str += p.toInfo().name + "\n";
  }
  if (!param_opts_str.empty())
    param_opts_str.pop_back();

  m_param_dd = Dropdown::create(*this);
  m_param_dd->set_options(param_opts_str.c_str()).set_width(LV_PCT(100));

  m_param_dd->on_value_changed([this]() { dispatch_if_changed(); });

  auto btn_cont = Component::create<Component>(*this);
  btn_cont->set_width(LV_PCT(100));
  btn_cont->set_height(LV_SIZE_CONTENT);
  btn_cont->set_style_bg_opa(Opa::Opa0);
  btn_cont->set_style_pad_all(0);

  lv_obj_set_flex_flow(btn_cont->raw(), LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(btn_cont->raw(), LV_FLEX_ALIGN_SPACE_BETWEEN,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_gap(btn_cont->raw(), 10, 0);

  m_save_btn = Button::create(*btn_cont);
  m_save_btn->set_width(LV_PCT(48))
      .set_height(50)
      .set_style_bg_color(Color::from_rgb(33, 150, 243));

  auto save_lbl = Label::create(*m_save_btn);
  save_lbl->set_text("Save").center();

  m_load_btn = Button::create(*btn_cont);
  m_load_btn->set_width(LV_PCT(48))
      .set_height(50)
      .set_style_bg_color(Color::from_rgb(33, 150, 243));

  auto load_lbl = Label::create(*m_load_btn);
  load_lbl->set_text("Load").center();

  load_values_to_ui();

  uint16_t c_idx = m_city_dd->get_selected();
  if (c_idx < kKnownCities.size())
    m_last_applied.city = kKnownCities[c_idx].station;

  uint16_t p_idx = m_param_dd->get_selected();
  if (p_idx < m_params.size())
    m_last_applied.parameter = m_params[p_idx];

  m_save_btn->on_clicked([this]() {
    uint16_t c_idx = m_city_dd->get_selected();
    if (c_idx < kKnownCities.size()) {
      m_current_settings.city = kKnownCities[c_idx].station;
    }

    uint16_t p_idx = m_param_dd->get_selected();
    if (p_idx < m_params.size()) {
      m_current_settings.parameter = m_params[p_idx];
    }

    SettingsStorage::save(m_current_settings);
    std::cout << "[SettingsUi] Defaults saved to disk (no fetch)." << std::endl;
  });

  m_load_btn->on_clicked([this]() {
    load_values_to_ui();

    dispatch_if_changed();
  });
}
