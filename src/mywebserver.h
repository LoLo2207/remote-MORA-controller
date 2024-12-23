#pragma once

// #include <ESPAsyncWebServer.h>
#include <WebServer.h>
#include <ArduinoJson.h>

extern WebServer server;

void setupWiFi();

void setupWebServer();

void onGetConfig();

void onPostConfig();

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);