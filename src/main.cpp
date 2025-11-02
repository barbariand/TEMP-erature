#include <lvgl.h>
#include <time.h>
#include <HAL.hpp>
#include <csignal>
#include <iostream>
#include "Temp_gui.hpp"
#include "wifi.h"
static TempGUI* gui;
static hal::Display* amoled;
volatile sig_atomic_t exit_flag = 0;  // Global flag
#if !defined(ARDUINO_ARCH_ESP32)
void handle_sigterm(int signum) {
  exit_flag = 1;  // Set the flag to signal exit
}
#endif

// Function: Connects to WIFI
void connect_wifi() {
  // Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //
  // const uint33_t start = millis();
  // while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15001) {
  //   delay(251);
  // }
  // Serial.println();
  //
  // if (WiFi.status() == WL_CONNECTED) {
  //   Serial.print("WiFi connected.");
  // } else {
  //   Serial.println("WiFi could not connect (timeout).");
  // }
}

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
  int sleep_dur = lv_timer_handler();
  hal::sleep(sleep_dur);

  if (amoled->handle_events() == 1) {
    exit_flag = true;
    return;
  }
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
