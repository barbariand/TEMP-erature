#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>
#include <time.h>

// Wi-Fi credentials (Delete these before commiting to GitHub)
static const char *WIFI_SSID = "SSID";
static const char *WIFI_PASSWORD = "PWD";

LilyGo_Class amoled;

static lv_obj_t *tileview;
static lv_obj_t *t1;
static lv_obj_t *t2;
static lv_obj_t *t3;
static lv_obj_t *t1_label;
static lv_obj_t *t2_label;
static lv_obj_t *chart_obj;       
static lv_chart_series_t *series; 
static bool t2_dark = false; // start tile #2 in light mode

#define POINT_COUNT 60
static int16_t chart_data[POINT_COUNT];

// Function: Tile #2 Color change
static void apply_tile_colors(lv_obj_t *tile, lv_obj_t *label, bool dark) {
  // Background
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(),0);

  // Text
  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(),0);
}

static void on_tile2_clicked(lv_event_t *e) {
  LV_UNUSED(e);
  t2_dark = !t2_dark;
  apply_tile_colors(t2, t2_label, t2_dark);
}

// Function: Creates UI
static void create_ui() {
  // Fullscreen Tileview
  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL),lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

  // Add two horizontal tiles
  t1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_ALL);
  t2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_ALL);
  t3 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_ALL);

  // Tile #1
  {
    t1_label = lv_label_create(t1);

    lv_label_set_text(t1_label, "Group 15, ver. 2.0");

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

  {
    // Tile #3: Chart
    chart_obj = lv_chart_create(t3);
    lv_obj_set_size(chart_obj, lv_disp_get_hor_res(NULL) - 20, lv_disp_get_ver_res(NULL) - 20);
    lv_obj_center(chart_obj);
    lv_chart_set_type(chart_obj, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart_obj, POINT_COUNT);
    lv_chart_set_range(chart_obj, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    // Initialize data array to 0
    for (int i = 0; i < POINT_COUNT; i++) chart_data[i] = 0;

    // Add series and link to external array
    series = lv_chart_add_series(chart_obj, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart_obj, series, chart_data);

    // Optional: label
    lv_obj_t *chart_label = lv_label_create(t3);
    lv_label_set_text(chart_label, "Live Data");
    lv_obj_set_style_text_font(chart_label, &lv_font_montserrat_20, 0);
    lv_obj_align(chart_label, LV_ALIGN_TOP_MID, 0, 5);

    lv_chart_refresh(chart_obj);
  }
}




// Function: Connects to WIFI
static void connect_wifi() {
  Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected.");
  } else {
    Serial.println("WiFi could not connect (timeout).");
  }
}

// Must have function: Setup is run once on startup
void setup() {
  Serial.begin(115200);
  delay(200);

  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true)
      delay(1000);
  }

  beginLvglHelperDMA(amoled); // init LVGL for this board

  create_ui();
  connect_wifi();
}

// Must have function: Loop runs continously on device after setup
void loop() {
  int sleep_delay = lv_timer_handler();
  delay(sleep_delay);
}
int main() {
  setup();
  while (true) {
    loop();
  }
}
