#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "common.h"

WiFiClient client;
HTTPClient http;
JsonDocument doc;

uint32_t nextUpdate = 0;

void init_sensors()
{
}

bool update_sensors(Data &data)
{
    if (nextUpdate < millis())
    {
        Serial.println("Update sensors");

        http.useHTTP10(true);
        http.begin(client, HA_URL);
        http.addHeader("Authorization", HA_TOKEN);
        http.GET();

        doc.clear();
        deserializeJson(doc, http.getStream());

        auto time = doc["attributes"]["time"].as<String>();
        data.hour = time.substring(0, 2).toInt();
        data.minute = time.substring(3, 5).toInt();
        data.tempSejour = doc["attributes"]["temp_sejour"].as<String>();
        data.tempExt = doc["attributes"]["temp_ext"].as<String>();
        data.humiSejour = doc["attributes"]["humi_sejour"].as<String>();
        data.humiExt = doc["attributes"]["humi_ext"].as<String>();

        Serial.print("Heure: ");
        Serial.print(data.hour);
        Serial.print(":");
        Serial.println(data.minute);

        Serial.print("Séjour: ");
        Serial.print(data.tempSejour);
        Serial.print(" ");
        Serial.println(data.humiSejour);

        Serial.print("Ext: ");
        Serial.print(data.tempExt);
        Serial.print(" ");
        Serial.println(data.humiExt);

        http.end();

        nextUpdate = millis() + UPDATE_INTERVAL_MS;

        return true;
    }

    return false;
}
