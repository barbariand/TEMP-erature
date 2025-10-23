#pragma once
#if defined(ARDUINO)
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
