#include <lvgl.h>
#include <time.h>

// Wi-Fi credentials (Delete these before commiting to GitHub)
static const char *WIFI_SSID = "SSID";
static const char *WIFI_PASSWORD = "PWD";
static uint32_t wifi_backoff_ms = 5000;
static uint32_t wifi_next_attempt =0;

LilyGo_Class amoled;

static lv_obj_t *tileview;
static lv_obj_t *t1;
static lv_obj_t *t2;
static lv_obj_t *t1_label;
static lv_obj_t *t2_label;
static bool t2_dark = false; // start tile #2 in light mode

// Function: Tile #2 Color change
static void apply_tile_colors(lv_obj_t *tile, lv_obj_t *label, bool dark) {
  // Background
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(),
                            0);

  // Text
  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(),
                              0);
}

static void on_tile2_clicked(lv_event_t *e) {
  LV_UNUSED(e);
  t2_dark = !t2_dark;
  apply_tile_colors(t2, t2_label, t2_dark);
}
#endif

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

static void wifi_reconnect_backoff(){
  if(WiFi.status() == WL_CONNECTED){
    wifi_backoff_ms = 5000;
    return;
  }

  uint32_t now = millis();
  if(now < wifi_next_attempt){
    return;
  }

  Serial.println("Wifi reconnecting...");

  WiFi.disconnect(true);
  delay(50);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
{}
   const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 30000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi reconnected.");
    wifi_backoff_ms = 5000;
    wifi_next_attempt = now + 10000;
    }
    else{
      wifi_next_attempt = now + wifi_backoff_ms;
      wifi_backoff_ms *= 2;
      if(wifi_backoff_ms > 120000){
        wifi_backoff_ms = 120000;
      }
    }
}

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
