#pragma once

#include "HalHelper.h"
#include "IDisplay.h"
#include "IInput.h"

#if defined(ARDUINO)
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "LilyGo/LV_Helper.h"
#include "LilyGoDisplay.h"
#include "LilyGoInput.h"
using Display = LilyGoDisplay;
#else
#include "SDLDisplay.h"
#include "SDLInput.h"
using Display = SDLDisplay;

#endif
