#include <lvgl.h>
#include <time.h>
#include <HAL.hpp>
#include <csignal>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "wifi.h"

hal::Display* amoled;
volatile sig_atomic_t exit_flag = 0;  // Global flag
#if !defined(ARDUINO_ARCH_ESP32)
void handle_sigterm(int signum) {
  exit_flag = 1;  // Set the flag to signal exit
}
#endif
static lv_obj_t* tileview;
static lv_obj_t* t1;
static lv_obj_t* t2;
static lv_obj_t* t1_label;
static lv_obj_t* t2_label;
static bool t2_dark = false;  // start tile #2 in light mode

// Function: Tile #2 Color change
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark) {
  std::cout << "lets goooooooo changing colour" << std::endl;
  // Background
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(),
                            0);

  // Text
  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(),
                              0);
}

static void on_tile2_clicked(lv_event_t* e) {
  LV_UNUSED(e);
  t2_dark = !t2_dark;
  apply_tile_colors(t2, t2_label, t2_dark);
}

// Function: Creates UI
static void create_ui() {
  // Fullscreen Tileview
  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL),
                  lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

  // Add two horizontal tiles
  t1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);

  // Tile #1
  {
    t1_label = lv_label_create(t1);
    lv_label_set_text(t1_label, "Hello Students");
    lv_obj_set_style_text_font(t1_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t1_label);
    apply_tile_colors(t1, t1_label, /*dark=*/false);
  }

  // Tile #2
  {
    t2_label = lv_label_create(t2);
    lv_label_set_text(t2_label, "Welcome to the workshop");
    lv_obj_set_style_text_font(t2_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t2_label);

    apply_tile_colors(t2, t2_label, /*dark=*/false);
    lv_obj_add_flag(t2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(t2, on_tile2_clicked, LV_EVENT_CLICKED, NULL);
  }
}

// Function: Connects to WIFI
static void connect_wifi() {
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

  create_ui();
  connect_wifi();
}

void loop() {
  std::unordered_map<int, int> test;
  if (exit_flag)
    return;

  if (amoled->handle_events() == 1) {
    exit_flag = true;
    return;
  }
  int sleep_dur = lv_timer_handler();
  hal::sleep(sleep_dur);
}
int main() {
  setup();

  while (!exit_flag) {
    loop();
  }
  std::cout<<"\nExiting gracefully..."<<std::endl;
  return 0;
}
