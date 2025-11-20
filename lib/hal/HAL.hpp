#pragma once
#include "Display.hpp"
#include "IDisplay.hpp"
#include <ArduinoJson.h>
namespace hal {
inline void init(Display* amoled);
inline void sleep(int sleep_delay);
}  // namespace hal
#if defined(LILYGO_BUILD)

#if !defined(ARDUINO_ARCH_ESP32)
#error LILYGO only builds to ARDUINO_ARCH_ESP32 target
#endif
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <lvgl.h>
#include "./LilyGo/LilyGo_AMOLED.h"
#include "ESP32Helper.hpp"
#include "LilyGo/LV_Helper.h"
#elif defined(SDL_BUILD)
#include "SDLHelper.hpp"
#elif defined(WASM_BUILD)
#include "WASMHelper.hpp"
#else
#error Enable one of SDL_BUILD, LILYGO_BUILD or WASM_BUILD
#endif
