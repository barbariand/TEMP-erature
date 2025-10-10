#pragma once
#include "IDisplay.h"
#if defined(ARDUINO)
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>
#include "./LilyGo/LilyGo_AMOLED.h"
#include "LilyGo/LV_Helper.h"
#include "LilyGoDisplay.h"
namespace hal {
using Display = LilyGoDisplay;
}
#include "EPS32Helper.h"
#else
#include "SDLDisplay.h"
namespace hal {
using Display = SDLDisplay;
}
#include "SDLHelper.h"
#endif
