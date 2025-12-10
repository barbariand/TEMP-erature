#if defined(LILYGO_BUILD)
#include "network.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <algorithm>
#include <iostream>
#include "wifi_cred.hpp"

static int wifi_backoff_ms = 5000;
static int wifi_next_attempt = 0;

void print_wifi_error(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD:
      std::cout << "Error: No WiFi shield present." << std::endl;
      break;
    case WL_IDLE_STATUS:
      std::cout << "WiFi idle, not connected." << std::endl;
      break;
    case WL_NO_SSID_AVAIL:
      std::cout << "Error: SSID not available." << std::endl;
      break;
    case WL_CONNECT_FAILED:
      std::cout << "Error: Connection failed." << std::endl;
      break;
    case WL_CONNECTION_LOST:
      std::cout << "Error: Connection lost." << std::endl;
      break;
    case WL_DISCONNECTED:
      std::cout << "Error: Disconnected." << std::endl;
      break;
    default:
      std::cout << "Unknown WiFi error." << std::endl;
      break;
  }
}

// Function: Connects to WIFI
bool connect_wifi() {
  std::cout << "Connecting to WiFi SSID: " << WIFI_SSID << std::endl;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const int start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(250);
  }
  std::cout << std::endl;

  if (WiFi.status() == WL_CONNECTED) {
    std::cout << "WiFi connected." << std::endl;
    // WiFi.localIP() returns an object, so we convert to string for std::cout
    std::cout << "IP address: " << WiFi.localIP().toString().c_str()
              << std::endl;
    
    // Set explicit DNS servers to resolve domain names
    IPAddress dns1(8, 8, 8, 8);        // Google DNS 1
    IPAddress dns2(8, 8, 4, 4);        // Google DNS 2
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), dns1, dns2);
    std::cout << "DNS servers configured." << std::endl;
    
    return true;
  } else {
    std::cout << "WiFi could not connect (timeout)." << std::endl;
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

  std::cout << "WiFi reconnecting..." << std::endl;

  WiFi.disconnect(true);
  delay(50);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 30000) {
    delay(250);
  }
  std::cout << std::endl;

  if (WiFi.status() == WL_CONNECTED) {
    std::cout << "WiFi reconnected." << std::endl;
    
    // Set explicit DNS servers to resolve domain names
    IPAddress dns1(8, 8, 8, 8);        // Google DNS 1
    IPAddress dns2(8, 8, 4, 4);        // Google DNS 2
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), dns1, dns2);
    std::cout << "DNS servers configured." << std::endl;
    
    wifi_backoff_ms = 5000;
    wifi_next_attempt = now + 10000;
  } else {
    // Replaced Serial.printf with stream insertion
    std::cout << "Reconnect failed after " << (millis() - start) << " ms"
              << std::endl;
    print_wifi_error(WiFi.status());

    wifi_next_attempt = now + wifi_backoff_ms;
    // Ensure we are comparing compatible types for min
    wifi_backoff_ms =
        std::min((uint32_t)(wifi_backoff_ms * 2), (uint32_t)120000);
  }
}
#else
void wifi_reconnect_backoff() {}

bool connect_wifi() {
  return true;
}
#endif
