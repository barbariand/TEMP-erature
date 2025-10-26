
#ifdef WASM_BUILD  // Guard for WASM-only compilation
#include "WASMDisplay.hpp"
namespace hal {
inline void sleep(int _sleep) {}
inline void init(Display* amoled) {
  if (!amoled->init()) {
  }
}
}  // namespace hal
#endif
