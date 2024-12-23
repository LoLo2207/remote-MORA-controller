#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

class Config {
public:
    String ssid;
    String password;
    uint8_t peerAddress[6];
    int interval;
    int fanDutyCycle;
    int fanPWMPin;
    int fanRPMPin;
    int pumpDutyCycle;
    int pumpPWMPin;
    int pumpRPMPin;

    Config(String ssid, String password, uint8_t peerAddress[6], int interval, int fanDutyCycle, int pumpDutyCycle) {
        this->ssid = ssid;
        this->password = password;
        memcpy(this->peerAddress, peerAddress, 6);
        this->interval = interval;
        this->fanDutyCycle = fanDutyCycle;
        this->fanPWMPin = fanPWMPin;
        this->fanRPMPin = fanRPMPin;
        this->pumpDutyCycle = pumpDutyCycle;
        this->pumpPWMPin = pumpPWMPin;
        this->pumpRPMPin = pumpRPMPin;
    }
};

extern Config config;

void setConfigObject(JsonDocument doc);

// void deserializeConfig(String body);
void deserializeConfig(File file);

JsonDocument serializeConfig();