#include <Arduino.h>
#include <WiFi.h>
// #include <ESPAsyncWebServer.h>
#include <esp_now.h>
#include <cJSON.h>
#include "myvariables.h"
#include "mywebserver.h"
#include "mysdcard.h"

uint8_t peerAddress[] = {0x24, 0x6F, 0x28, 0xAE, 0x1C, 0xD4}; // Replace with the MAC address of the peer
int dutyCycle = 0;
int rpmValue = 0;

Config config = Config(String(), String(), (uint8_t[6]){0, 0, 0, 0, 0, 0}, 1000, 50, 50);

void setup()
{
    Serial.begin(115200);

    setupSDCard();

    loadConfigFromFile("/config.json");

    setupWiFi();

    setupWebServer();

    // esp_now_peer_info_t peerInfo;
    // memcpy(peerInfo.peer_addr, peerAddress, 6);
    // peerInfo.channel = 0;
    // peerInfo.encrypt = false;

    // if (esp_now_add_peer(&peerInfo) != ESP_OK)
    // {
    //     Serial.println("Failed to add peer");
    //     return;
    // }

    server.begin();
}

void loop()
{
    server.handleClient();

}
