
#pragma once
#include <GUI.hpp>
#include "ForcastUiItem.hpp"
#include "api/api.hpp"
using namespace LVGL_Wrapper;
class ForcastUI : public Component {
  std::shared_ptr<Label> city_label;
  std::vector<std::shared_ptr<ForcastUIItem>> items;

 public:
  void init() override;
  explicit ForcastUI(BaseWidget* parent) : Component(parent) {}
  void update(ForecastSevenDay data);
};
