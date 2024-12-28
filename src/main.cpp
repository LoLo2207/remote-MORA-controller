#include <Arduino.h>
#include <WiFi.h>
// #include <ESPAsyncWebServer.h>
#include <esp_now.h>
#include <cJSON.h>
#include "myconfig.h"
#include "mywebserver.h"
#include "mysdcard.h"
#include "pwm_rpm.h"

// uint8_t peerAddress[] = {0x24, 0x6F, 0x28, 0xAE, 0x1C, 0xD4}; // Replace with the MAC address of the peer

unsigned long previousFanMillis;
unsigned long previousPumpMillis;
int lastFanRPM;
int lastPumpRPM;
volatile unsigned long fanPulseCount;
volatile unsigned long pumpPulseCount;

Config config = Config(String(), String(), 1000, 50, 50);

void ISR_onFanPulse()
{
    fanPulseCount++;
}

void ISR_onPumpPulse()
{
    pumpPulseCount++;
}


void calculateFanRPM()
{
    detachInterrupt(digitalPinToInterrupt(config.fanRPMPin));
    //rpm = (freq(Hz) / 2) * 60 = freq(Hz) * 30
    lastFanRPM = (fanPulseCount * 30) / 8 / 1000;
    fanPulseCount = 0;
    if(lastFanRPM != 0)
    {
        Serial.print("Fan RPM: ");
        Serial.println(lastFanRPM);
    }
    attachInterrupt(digitalPinToInterrupt(config.fanRPMPin), ISR_onFanPulse, RISING);
}

void calculatePumpRPM()
{
    detachInterrupt(digitalPinToInterrupt(config.pumpRPMPin));
    //rpm = (freq(Hz) / 2) * 60 = freq(Hz) * 30
    lastPumpRPM = (pumpPulseCount * 30) / 1000;
    pumpPulseCount = 0;
    if(lastPumpRPM != 0)
    {
        Serial.print("Pump RPM: ");
        Serial.println(lastPumpRPM);
        Serial.println();
    }
    attachInterrupt(digitalPinToInterrupt(config.pumpRPMPin), ISR_onPumpPulse, RISING);
}

void setup()
{
    Serial.begin(115200);

    setupSDCard();

    loadConfigFromFile("/config.json");

    JsonDocument doc = serializeConfig();

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

    // PWM GENERATION SETUP
    Serial.println("Setting up PWM pins");

    pinMode(config.fanPWMPin, OUTPUT);
    // pinMode(config.pumpPWMPin, OUTPUT);

    ledcSetup(0, 25000, 8);  // Channel 0, 25kHz, 8-bit resolution
    // ledcSetup(1, 25000, 8);  // Channel 1, 25kHz, 8-bit resolution

    ledcAttachPin(config.fanPWMPin, 0);
    // ledcAttachPin(config.pumpPWMPin, 1);

    ledcWrite(0, config.fanDutyCycle);
    // ledcWrite(1, config.pumpDutyCycle);

    Serial.println("PWM pins set up");


    // RPM READING SETUP
    Serial.println("Setting up RPM pins");
    pinMode(config.fanRPMPin, INPUT_PULLUP);
    pinMode(config.pumpRPMPin, INPUT_PULLUP);
    attachInterrupt(config.fanRPMPin, ISR_onFanPulse, RISING);
    attachInterrupt(digitalPinToInterrupt(config.pumpRPMPin), ISR_onPumpPulse, RISING);
    previousFanMillis = 0;
    previousPumpMillis = 0;
    lastFanRPM = 0;
    lastPumpRPM = 0;
    fanPulseCount = 0;
    pumpPulseCount = 0;
    Serial.println("RPM pins set up");

    Serial.println("Setup done");
}

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

