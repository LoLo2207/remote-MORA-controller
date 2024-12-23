#pragma once

#include <Arduino.h>
#include <SD.h>

void setupSDCard();

void loadConfigFromFile(String path);

void saveConfigToFile(String path);