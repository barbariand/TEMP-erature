#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

#include "api/cities.hpp"
#include "api/data/DataPoint.hpp"
#include "api/data/ForcastSevenDays.hpp"
#include "api/data/ObservationSeries.hpp"
#include "api/parameters/MeterologyCode.hpp"
#include "api/parameters/SevenDayForcastParameters.hpp"
#include "api/smhi_client.hpp"
#include "api/url/SMHIUrlGenerator.hpp"
#include "lvgl.h"
#include "network/network.hpp"
#include "settings_storage.hpp"
#include "types/Enums.hpp"

using namespace LVGL_Wrapper;

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();

  m_tileview = Tileview::create(*screen);
  m_tileview->set_size(LV_PCT(100), LV_PCT(100));
  m_tileview->set_scrollbar_mode(ScrollbarMode::Off);

  m_t0_group = m_tileview->add_tile(0, 0, Direction::Right);
  m_c0_group = Component::create<MainUi>(*m_t0_group);

    };
  }

  Settings s = SettingsStorage::load();

  }
}
