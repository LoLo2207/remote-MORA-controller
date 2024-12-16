#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_now.h>
#include <cJSON.h>

AsyncWebServer server(80);

uint8_t peerAddress[] = {0x24, 0x6F, 0x28, 0xAE, 0x1C, 0xD4}; // Replace with the MAC address of the peer
int dutyCycle = 0;
int rpmValue = 0;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&rpmValue, incomingData, sizeof(rpmValue));
    Serial.print("RPM Value: ");
    Serial.println(rpmValue);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, peerAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    server.on("/setDutyCycle", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("dutyCycle", true)) {
            dutyCycle = request->getParam("dutyCycle", true)->value().toInt();
            Serial.print("Duty Cycle: ");
            Serial.println(dutyCycle);
            esp_now_send(peerAddress, (uint8_t *)&dutyCycle, sizeof(dutyCycle));

            cJSON *response = cJSON_CreateObject();
            cJSON_AddStringToObject(response, "status", "Duty Cycle set");
            char *responseString = cJSON_Print(response);
            request->send(200, "application/json", responseString);
            cJSON_Delete(response);
            free(responseString);
        } else {
            cJSON *response = cJSON_CreateObject();
            cJSON_AddStringToObject(response, "status", "Bad Request");
            char *responseString = cJSON_Print(response);
            request->send(400, "application/json", responseString);
            cJSON_Delete(response);
            free(responseString);
        }
    });

    server.on("/getRPM", HTTP_GET, [](AsyncWebServerRequest *request) {
        cJSON *response = cJSON_CreateObject();
        cJSON_AddNumberToObject(response, "rpm", rpmValue);
        char *responseString = cJSON_Print(response);
        request->send(200, "application/json", responseString);
        cJSON_Delete(response);
        free(responseString);
    });

    server.begin();
}

void loop() {
}
