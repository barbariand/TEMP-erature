#pragma once
// Wi-Fi credentials (Delete these before commiting to GitHub)
static const char* WIFI_SSID = "SSID";
static const char* WIFI_PASSWORD = "PWD";

bool connect_wifi();

void wifi_reconnect_backoff();
