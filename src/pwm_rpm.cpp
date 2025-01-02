#include <Arduino.h>
#include "pwm_rpm.h"
#include "myconfig.h"

extern  unsigned long previousFanMillis;
extern  unsigned long previousPumpMillis;
extern  int lastFanRPM;
extern  int lastPumpRPM;
extern  unsigned long fanPulseCount;
extern  unsigned long pumpPulseCount;

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

void ISR_onFanPulse()
{
    fanPulseCount++;
}

void ISR_onPumpPulse()
{
    pumpPulseCount++;
}

void setupPWM()
{
    // PWM GENERATION SETUP
    Serial.println("Setting up PWM pins");

    pinMode(config.fanPWMPin, config.fanDutyCycle > 0 ? OUTPUT : INPUT);
    pinMode(config.pumpPWMPin, config.pumpDutyCycle > 0 ? OUTPUT : INPUT);

    ledcSetup(0, 25000, 8);  // Channel 0, 25kHz, 8-bit resolution
    ledcSetup(1, 25000, 8);  // Channel 1, 25kHz, 8-bit resolution

    ledcAttachPin(config.fanPWMPin, 0);
    ledcAttachPin(config.pumpPWMPin, 1);

    uint32_t fanDutyCycle = map(config.fanDutyCycle, 0, 100, 0, 255);
    uint32_t pumpDutyCycle = map(config.pumpDutyCycle, 0, 100, 0, 255);

    ledcWrite(0, fanDutyCycle);
    ledcWrite(1, pumpDutyCycle);

    Serial.println("PWM pins set up\n");


    // RPM READING SETUP
    Serial.println("Setting up RPM pins");
    pinMode(config.fanRPMPin, INPUT_PULLUP);
    pinMode(config.pumpRPMPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(config.fanRPMPin), ISR_onFanPulse, RISING);
    attachInterrupt(digitalPinToInterrupt(config.pumpRPMPin), ISR_onPumpPulse, RISING);
    previousFanMillis = 0;
    previousPumpMillis = 0;
    lastFanRPM = 0;
    lastPumpRPM = 0;
    fanPulseCount = 0;
    pumpPulseCount = 0;
    Serial.println("RPM pins set up");
}