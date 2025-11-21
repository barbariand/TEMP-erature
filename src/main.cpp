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

// Must have function: Setup is run once on startup
void setup() {
Serial.begin(115200);
#if !defined(ARDUINO_ARCH_ESP32)
  signal(SIGTERM, handle_sigterm);
  signal(SIGINT, handle_sigterm);
#endif
  std::cout << "start" << std::endl;
  amoled = new hal::Display();
  hal::init(amoled);

  gui = new TempGUI();
  gui->create_ui();
  lv_obj_invalidate(lv_scr_act());
}

void loop() {


  int sleep_delay = lv_timer_handler();
  delay(sleep_delay);

  #if defined(ARDUINO_ARCH_ESP32)
    // Läs från Serial Monitor och uppdatera grafen
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n'); // Läs en rad
        int value = input.toInt();                   // Konvertera till int
        gui->update_chart(value);                    // Uppdatera grafen
    }
  #endif
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
