#pragma once
#include <GUI.hpp>
#include "ForcastUiItem.hpp"
#include "api/api.hpp"

class ForcastUI : public LVGL_Wrapper::Component {
  std::shared_ptr<LVGL_Wrapper::Label> city_label;
  std::vector<std::shared_ptr<ForcastUIItem>> items;

 public:
  explicit ForcastUI(LVGL_Wrapper::BaseWidget* parent)
      : LVGL_Wrapper::Component(parent) {}

  void init() override;

  void update(const ForecastSevenDay& data);

  void set_city(const char* city);
};
