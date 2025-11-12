#pragma once
#if defined(LILYGO_BUILD)
#include "LilyGoDisplay.hpp"
namespace hal {
using Display = LilyGoDisplay;
}
#elif defined(SDL_BUILD)
#include "SDLDisplay.hpp"
namespace hal {
using Display = SDLDisplay;
}
#elif defined(WASM_BUILD)
#include "WASMDisplay.hpp"
namespace hal {
using Display = WASMDisplay;
}
#endif
