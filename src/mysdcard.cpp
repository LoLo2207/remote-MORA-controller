#include <SD.h>
#include <ArduinoJson.h>
#include "mysdcard.h"
#include "myvariables.h"

void setupSDCard()
{
    if (!SD.begin())
    {
        Serial.println("Error initializing SD card");
        return;
    }

    Serial.println("SD card initialized");
}

void loadConfigFromFile(String path)
{
    File file = SD.open(path);
    if (!file)
    {
        Serial.println("Failed to open config.json");
        return;
    }

    deserializeConfig(file);    

    file.close();
}

void saveConfigToFile(String path)
{
    File file = SD.open(path, FILE_WRITE, true);
    if (!file)
    {
        Serial.println("Failed to open config.json for writing");
        return;
    }

    JsonDocument doc = serializeConfig();

    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Failed to write to config.json");
    }
    else
    {
        Serial.println("Config saved");
    }

    file.close();
}