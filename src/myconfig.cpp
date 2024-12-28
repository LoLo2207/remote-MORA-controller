#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "myconfig.h"

// void deserializeConfig(String body)
// {
//     Serial.println("Deserializing config body");
//
//     JsonDocument doc;
//     DeserializationError error = deserializeJson(doc, body);
//     if (error)
//     {
//         Serial.print("Failed to parse JSON: ");
//         Serial.println(error.c_str());
//     }
//
//     setConfigObject(doc);
// }

void deserializeConfig(File file)
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

    setConfigObject(doc);

    updatePWM
}


void setConfigObject(JsonDocument doc)
{
    Serial.println("=== Setting config object ===");
    //print all config
    for (JsonPair kv : doc.as<JsonObject>())
    {
        Serial.print("    ");
        Serial.print(kv.key().c_str());
        Serial.print(": ");
        Serial.println(kv.value().as<String>());
    }

    config.ssid = doc["ssid"].as<String>();
    config.password = doc["password"].as<String>();

    config.fanPWMPin = doc["fanPWMPin"].as<int>();
    config.fanRPMPin = doc["fanPWMPin"].as<int>();
    config.fanDutyCycle = doc["fanDutyCycle"].as<int>();

    config.pumpPWMPin = doc["pumpPWMPin"].as<int>();
    config.pumpRPMPin = doc["pumpRPMPin"].as<int>();
    config.pumpDutyCycle = doc["pumpDutyCycle"].as<int>();
    
    Serial.println("=== Setting config object ===");
}

JsonDocument serializeConfig()
{
    Serial.println("Serializing config object");

    JsonDocument doc;
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;

    doc["fanPWMPin"] = config.fanPWMPin;
    doc["fanRPMPin"] = config.fanRPMPin;
    doc["fanDutyCycle"] = config.fanDutyCycle;

    doc["pumpPWMPin"] = config.pumpPWMPin;
    doc["pumpRPMPin"] = config.pumpRPMPin;
    doc["pumpDutyCycle"] = config.pumpDutyCycle;

    return doc;
}