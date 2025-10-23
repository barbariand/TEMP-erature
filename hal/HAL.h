#pragma once
#include "Display.h"
#include "IDisplay.h"
#if defined(ARDUINO)
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>
#include "./LilyGo/LilyGo_AMOLED.h"
#include "EPS32Helper.h"
#include "LilyGo/LV_Helper.h"
#else
#include "SDLHelper.h"
#endif
