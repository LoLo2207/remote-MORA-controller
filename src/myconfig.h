#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

class Config {
public:
    String ssid;
    String password;
    
    // Intervalo entre lecturas de sensores, EN MILISENGUNDOS
    int interval;

    int fanDutyCycle;
    int fanPWMPin;
    int fanRPMPin;
    int pumpDutyCycle;
    int pumpPWMPin;
    int pumpRPMPin;

    Config(String ssid, String password, int interval, int fanDutyCycle, int pumpDutyCycle) {
        this->ssid = ssid;
        this->password = password;
        this->interval = interval;
        this->fanDutyCycle = fanDutyCycle;
        this->fanPWMPin = fanPWMPin;
        this->fanRPMPin = fanRPMPin;
        this->pumpDutyCycle = pumpDutyCycle;
        this->pumpPWMPin = pumpPWMPin;
        this->pumpRPMPin = pumpRPMPin;
    }

    JsonDocument ToJSON()
    {
        JsonDocument doc;
        // doc["ssid"] = this->ssid;
        // doc["password"] = this->password;

        doc["fanPWMPin"] = this->fanPWMPin;
        doc["fanRPMPin"] = this->fanRPMPin;
        doc["fanDutyCycle"] = this->fanDutyCycle;

        doc["pumpPWMPin"] = this->pumpPWMPin;
        doc["pumpRPMPin"] = this->pumpRPMPin;
        doc["pumpDutyCycle"] = this->pumpDutyCycle;

        return doc;
    }

    void FromJSON(JsonDocument doc)
    {
        // for (JsonPair kv : doc.as<JsonObject>())
        // {
        //     Serial.print("    ");
        //     Serial.print(kv.key().c_str());
        //     Serial.print(": ");
        //     Serial.println(kv.value().as<String>());
        // }

        this->ssid = doc["ssid"].as<String>();
        this->password = doc["password"].as<String>();

        this->fanPWMPin = doc["fanPWMPin"];
        this->fanRPMPin = doc["fanRPMPin"];
        this->fanDutyCycle = doc["fanDutyCycle"];

        this->pumpPWMPin = doc["pumpPWMPin"];
        this->pumpRPMPin = doc["pumpRPMPin"];
        this->pumpDutyCycle = doc["pumpDutyCycle"];
    }

    void FromFile(File file)
    {
        Serial.println("Deserializing config file");

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, file);
        if (error)
        {
            Serial.print("Failed to parse config.json: ");
            Serial.println(error.c_str());
            return;
        }

        FromJSON(doc);
    }
};

extern Config config;