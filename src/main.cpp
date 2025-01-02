#include <Arduino.h>
#include <WiFi.h>
#include <cJSON.h>
#include "myconfig.h"
#include "mywebserver.h"
#include "mysdcard.h"
#include "pwm_rpm.h"

unsigned long previousFanMillis;
unsigned long previousPumpMillis;
int lastFanRPM;
int lastPumpRPM;
volatile unsigned long fanPulseCount;
volatile unsigned long pumpPulseCount;

Config config = Config(String(), String(), 1000, 50, 50);

/*******************/
/****** SETUP ******/
/*******************/
void setup()
{
    Serial.begin(115200);

    setupSDCard();

    loadConfigFromFile("/config.json");

    setupWiFi();

    setupWebServer();

    server.begin();

    setupPWM();

    Serial.println("Setup done");
}

/******************/
/****** LOOP ******/
/******************/
void loop()
{
    
    server.handleClient();
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousFanMillis >= config.interval)
    {
        previousFanMillis = currentMillis;
        calculateFanRPM();
    }

    if (currentMillis - previousPumpMillis >= config.interval)
    {
        previousPumpMillis = currentMillis;
        calculatePumpRPM();
    }
   
}

