#include <Arduino.h>
#include <EEPROM.h>
#include "handleJSON.h"
#include "handleHttp.h"
#include "handleWifi.h"
#include "handleMQTT.h"

void loadData(){
    EEPROM.begin(512);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    char ok[2 + 1];
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server),ok);
    EEPROM.end();
    if (String(ok) != String("OK"))
    {
        ssid[0] = 0;
        password[0] = 0;
        firstBoot = true;
    }
    Serial.println("Recovered credentials:");
    Serial.println(ssid);
    Serial.println(strlen(password) > 0 ? "********" : "<no password>");
    Serial.println("Finished loading data");
    Serial.println(mqtt_server);
    Serial.println(myHostname);
    setupMQTT();
}

void saveData(){
    EEPROM.begin(512);
    EEPROM.put(0, ssid);
    EEPROM.put(0 + sizeof(ssid), password);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server),ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials and other information");
    Serial.println(mqtt_server);
    Serial.println(myHostname);
    ESP.restart();
}