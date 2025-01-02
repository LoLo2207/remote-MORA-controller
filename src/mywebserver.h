#pragma once

#include <WebServer.h>
#include <ArduinoJson.h>

extern WebServer server;

void setupWiFi();

void setupWebServer();

void onGetConfig();

void onPostConfig();
