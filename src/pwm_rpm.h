// #pragma once

// extern unsigned long previousFanMillis;
// extern unsigned long previousPumpMillis;
// extern int lastFanRPM;
// extern int lastPumpRPM;
// extern volatile unsigned long fanPulseCount;
// extern volatile unsigned long pumpPulseCount;


void calculateFanRPM();
void calculatePumpRPM();

void ISR_onFanPulse();
void ISR_onPumpPulse();
void setupPWM();