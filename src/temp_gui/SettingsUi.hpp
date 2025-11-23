#pragma once
#include <GUI.hpp>
#include "settings_storage.hpp"

using namespace LVGL_Wrapper;

class SettingsUI : public Component {
  std::shared_ptr<Label> title_label;

  // City selection
  std::shared_ptr<Label> city_label;
  std::shared_ptr<Dropdown> city_dropdown;
  std::vector<std::string> city_list;

  // Unit selection
  // Parameter selection (for historical graph)
  std::shared_ptr<Label> parameter_label;
  std::shared_ptr<Dropdown> parameter_dropdown;
  std::vector<std::string> parameter_list;

  // Save button
  std::shared_ptr<Button> save_button;

 public:
  // Callback invoked after successful save so parent UI can apply changes
  std::function<void(const Settings&)> on_save_callback;
  void init() override;
  void load_values();
  explicit SettingsUI(BaseWidget* parent) : Component(parent) {}
};
