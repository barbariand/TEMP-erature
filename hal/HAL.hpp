#pragma once
#include "Display.hpp"
#include "IDisplay.hpp"
#if defined(ARDUINO_ARCH_ESP32)
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>
#include "./LilyGo/LilyGo_AMOLED.h"
#include "EPS32Helper.hpp"
#include "LilyGo/LV_Helper.h"
#else
#include "SDLHelper.hpp"
#endif
