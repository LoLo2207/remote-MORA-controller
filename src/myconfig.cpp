#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "myconfig.h"

// #ifndef CONFIG_H
// #define CONFIG_H

// class Config {
//     String ssid;
//     String password;
    
//     // Intervalo entre lecturas de sensores, EN MILISENGUNDOS
//     int interval;

//     int fanDutyCycle;
//     int fanPWMPin;
//     int fanRPMPin;
//     int pumpDutyCycle;
//     int pumpPWMPin;
//     int pumpRPMPin;

// public:
//     Config(String ssid, String password, int interval, int fanDutyCycle, int pumpDutyCycle);
//     JsonDocument ToJSON();
//     void FromJSON(JsonDocument doc);
//     void FromFile(File file);
// };

// #endif