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

    };
  }

  Settings s = SettingsStorage::load();

  }
}
