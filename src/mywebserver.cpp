#include <WebServer.h>
#include <esp_now.h>
#include <ArduinoJson.h>
// #include <cJSON.h>
#include "myvariables.h"
#include "mywebserver.h"
#include "mysdcard.h"

WebServer server(80);

void setupWiFi()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(config.ssid, config.password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(".");
    }
    Serial.println("Connected to WiFi");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // if (esp_now_init() != ESP_OK)
    // {
    //     Serial.println("Error initializing ESP-NOW");
    //     return;
    // }

    // esp_now_register_send_cb(onDataSent);
    // esp_now_register_recv_cb(onDataRecv);

    // Serial.println("ESP-NOW Initialized");

    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void setupWebServer()
{
    #pragma region WEB SERVER ROUTES
    
    // server.on("/setDutyCycle", HTTP_POST, [](AsyncWebServerRequest *request)
    // {
    //     if (request->hasParam("dutyCycle", true)) {
    //         dutyCycle = request->getParam("dutyCycle", true)->value().toInt();
    //         Serial.print("Duty Cycle: ");
    //         Serial.println(dutyCycle);
    //         esp_now_send(peerAddress, (uint8_t *)&dutyCycle, sizeof(dutyCycle));

    //         cJSON *response = cJSON_CreateObject();
    //         cJSON_AddStringToObject(response, "status", "Duty Cycle set");
    //         char *responseString = cJSON_Print(response);
    //         request->send(200, "application/json", responseString);
    //         cJSON_Delete(response);
    //         free(responseString);
    //     } else {
    //         cJSON *response = cJSON_CreateObject();
    //         cJSON_AddStringToObject(response, "status", "Bad Request");
    //         char *responseString = cJSON_Print(response);
    //         request->send(400, "application/json", responseString);
    //         cJSON_Delete(response);
    //         free(responseString);
    //     }
    // });

    // server.on("/getRPM", HTTP_GET, [](AsyncWebServerRequest *request)
    // {
    //     cJSON *response = cJSON_CreateObject();
    //     cJSON_AddNumberToObject(response, "rpm", rpmValue);
    //     char *responseString = cJSON_Print(response);
    //     request->send(200, "application/json", responseString);
    //     cJSON_Delete(response);
    //     free(responseString);
    // });

    server.on("/config", HTTP_GET, onGetConfig);
    
    server.on("/config", HTTP_POST, onPostConfig);
    
    server.on("/start", HTTP_GET, []()
    {
      server.send(200, "application/json", "Called /start");
    });

    server.on("/stop", HTTP_GET, []()
    {
      server.send(200, "application/json", "Called /stop");
    });

    #pragma endregion WEB SERVER ROUTES

  server.begin();
  Serial.println("Web server started");
}

#pragma region WEB SERVER HANDLERS

/**
 * Called when a GET request is made to /config, to get the current configuration of the device
 */
void onGetConfig()
{
  JsonDocument doc = serializeConfig();

  String response;
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

/**
 * Called when a POST request is made to /config, to update the configuration in the device
 */
void onPostConfig()
{
  if (server.hasArg("plain"))
  {
    String body = server.arg("plain");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (error)
    {
      Serial.print("Failed to parse request body: ");
      Serial.println(error.c_str());
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    if (doc["interval"].is<uint>())
    {
      config.interval = doc["interval"];
    }

    // if (doc["ssid"].is<String>())
    // {  
    //   config.ssid = doc["ssid"].as<String>();
    // }

    // if (doc["password"].is<String>())
    // {
    //   config.password = doc["password"].as<String>();
    // }

    if (doc["fanPWMPin"].is<int>())
    {
      config.fanPWMPin = doc["fanPWMPin"];
    }

    if (doc["fanRPMPin"].is<int>())
    {
      config.fanRPMPin = doc["fanRPMPin"];
    }

    if (doc["fanDutyCycle"].is<int>())
    {
      config.fanDutyCycle = doc["fanDutyCycle"];
    }

    if (doc["pumpPWMPin"].is<int>())
    {
      config.pumpPWMPin = doc["pumpPWMPin"];
    }

    if (doc["pumpRPMPin"].is<int>())
    {
      config.pumpRPMPin = doc["pumpRPMPin"];
    }

    if (doc["pumpDutyCycle"].is<int>())
    {
      config.pumpDutyCycle = doc["pumpDutyCycle"];
    }

    saveConfigToFile("/config.json");

    server.send(200, "text/plain", "Config updated");
  }
  else
  {
    server.send(400, "text/plain", "Body not provided");
  }
}



#pragma endregion WEB SERVER HANDLERS

#pragma region ESP-NOW CALLBACKS

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    // memcpy(&rpmValue, incomingData, sizeof(rpmValue));
    // Serial.print("RPM Value: ");
    // Serial.println(rpmValue);
}

#pragma endregion ESP-NOW CALLBACKS