#pragma once
#include <GUI.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../settings_storage.hpp"
#include "api/cities.hpp"
#include "api/parameters/MeterologyCode.hpp"

struct SettingsData {
  int city = 0;
  MeterologyCode parameter = MeterologyCode::AirTemperature_DailyMean;
};

using namespace LVGL_Wrapper;

class SettingsUi : public Component {

  Settings m_current_settings;
  SettingsData m_last_applied;

  std::vector<std::string> m_city_names;
  std::vector<MeterologyCode> m_params;

  std::shared_ptr<Label> m_title;
  std::shared_ptr<Label> m_city_label;
  std::shared_ptr<Dropdown> m_city_dd;
  std::shared_ptr<Label> m_param_label;
  std::shared_ptr<Dropdown> m_param_dd;

  std::shared_ptr<Button> m_save_btn;
  std::shared_ptr<Button> m_load_btn;
  std::shared_ptr<Button> m_reset_btn;

  void dispatch_if_changed();

 public:
  std::function<void(const SettingsData&)> on_save;

  explicit SettingsUi(BaseWidget* parent);
  void init() override;
  void load_values_to_ui();
};
