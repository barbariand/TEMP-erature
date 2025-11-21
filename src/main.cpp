#include <lvgl.h>
#include <HAL.hpp>
#include <csignal>
#include <iostream>
#include "network/network.hpp"
#include "temp_gui/Temp_gui.hpp"
// statics
static TempGUI* gui;
static hal::Display* amoled;
volatile sig_atomic_t exit_flag = 0;

// Must have function: Setup is run once on startup
void setup() {
  std::cout << "start" << std::endl;
  amoled = new hal::Display();
  // hal::init runs Serial.begin(115200);
  hal::init(amoled);

  gui = new TempGUI();
  gui->create_ui();
  lv_obj_invalidate(lv_scr_act());
}

void loop() {
  //wifi_reconnect_backoff();
  int sleep_delay = lv_timer_handler();
  hal::sleep(sleep_delay);

}
//exclude main from testin
#ifndef PIO_UNIT_TESTING
int main() {
  setup();

  while (!exit_flag) {
    loop();
  }
  std::cout << "\nExiting gracefully..." << std::endl;
  return 0;
}
#endif
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
