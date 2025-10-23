#pragma once
#if defined(ARDUINO)
#include "LilyGoDisplay.h"
namespace hal {
using Display = LilyGoDisplay;
}
#else
#include "SDLDisplay.h"
namespace hal {
using Display = SDLDisplay;
}
#endif
