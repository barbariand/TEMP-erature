#include "wifi.h"
#include <Arduino.h>
#include <WiFi.h>

static uint32_t wifi_backoff_ms = 5000;
static uint32_t wifi_next_attempt = 0;

void print_wifi_error(wl_status_t status) {
    switch (status){
    case WL_NO_SHIELD:   Serial.println("Error: No WiFi shield present."); break;
    case WL_IDLE_STATUS: Serial.println("WiFi idle, not connected."); break;
    case WL_NO_SSID_AVAIL: Serial.println("Error: SSID not available."); break;
    case WL_CONNECT_FAILED: Serial.println("Error: Connection failed."); break;
    case WL_CONNECTION_LOST: Serial.println("Error: Connection lost."); break;
    case WL_DISCONNECTED: Serial.println("Error: Disconnected."); break;
    default: Serial.println("Unknown WiFi error."); break;
    }
}


// Function: Connects to WIFI
bool connect_wifi() {
  Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("WiFi could not connect (timeout).");
    print_wifi_error(WiFi.status());
    return false;
  }
}

void wifi_reconnect_backoff() {
  if (WiFi.status() == WL_CONNECTED) {
    wifi_backoff_ms = 5000;
    return;
  }

  uint32_t now = millis();
  if (now < wifi_next_attempt) {
    return;
  }

  Serial.println("WiFi reconnecting...");

  WiFi.disconnect(true);
  delay(50);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 30000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi reconnected.");
    wifi_backoff_ms = 5000;
    wifi_next_attempt = now + 10000;
  } else {
    Serial.printf("Reconnect failed after %lu ms\n", (millis() - start));
    print_wifi_error(WiFi.status());
    wifi_next_attempt = now + wifi_backoff_ms;
    wifi_backoff_ms = min(wifi_backoff_ms *2, (uint32_t)120000);
    }
}
