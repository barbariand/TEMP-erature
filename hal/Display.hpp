#pragma once
#if defined(ARDUINO_ARCH_ESP32)
#include "LilyGoDisplay.hpp"
namespace hal {
using Display = LilyGoDisplay;
}
#else
#include "SDLDisplay.hpp"
namespace hal {
using Display = SDLDisplay;
}
#endif
