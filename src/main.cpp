#include <lvgl.h>
#include <HAL.hpp>
#include <csignal>
#include <iostream>
#include "Temp_gui.hpp"
#include "network.h"
#include "wifi_cred.h"
// statics
static TempGUI* gui;
static hal::Display* amoled;
volatile sig_atomic_t exit_flag = 0;
#if !defined(ARDUINO_ARCH_ESP32)
#endif

// Must have function: Setup is run once on startup
void setup() {

#if !defined(ARDUINO_ARCH_ESP32)
  signal(SIGTERM, handle_sigterm);
  signal(SIGINT, handle_sigterm);
#endif
  amoled = new hal::Display();
  hal::init(amoled);
  gui = new TempGUI();
  gui->create_ui();
  lv_obj_invalidate(lv_scr_act());
  connect_wifi();
}

void loop() {
  wifi_reconnect_backoff();

  int sleep_delay = lv_timer_handler();
  delay(sleep_delay);
}
int main() {
  setup();

  while (!exit_flag) {
    loop();
  }
  std::cout << "\nExiting gracefully..." << std::endl;
  return 0;
}
#if defined(WASM_BUILD)
extern "C" {
void app_setup() {
  setup();
}

void app_loop() {
  loop();
}
}
#endif  // WASM_BUILD
