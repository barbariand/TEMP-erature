#pragma once
#include <GUI.hpp>
#include "userprefs.hpp"
#include "api/Cities.hpp"
#include "temp_gui/settings_storage.hpp"

using namespace LVGL_Wrapper;

class SettingsUI : public Component {
  std::shared_ptr<Label> title_label;

  // City selection
  std::shared_ptr<Label> place_label;
  std::shared_ptr<Dropdown> place_dropdown;

  // Parameter selection
  std::shared_ptr<Label> metric_label;
  std::shared_ptr<Dropdown> metric_dropdown;
  std::vector<MeterologyCode> metric_list;

  // Save button
  std::shared_ptr<Button> save_button;

 public:
  std::function<void(const Settings&)> on_save_callback;
  void init() override;
  void load_values();
  explicit SettingsUI(BaseWidget* parent) : Component(parent) {}
};
