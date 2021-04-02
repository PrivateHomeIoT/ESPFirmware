#include <Arduino.h>
#include <EEPROM.h>
#include "handleData.h"
#include "handleEncryption.h"
#include "handleHttp.h"
#include "handleWifi.h"
#include "handleMQTT.h"
#include "handlePorts.h"
#include <ArduinoJson.h>

JsonObject parseJSON(char* rawJson){
    DynamicJsonDocument doc(1024);
    deserializeJson(doc,rawJson);
    JsonObject obj = doc.as<JsonObject>();
    return obj;
}

void serialSetup(){
    Serial.begin(9600);
    if(Serial.available()){
        Serial.flush();
        Serial.println("PrivateHome-ESPFirmware");
        Serial.println();
    }
}

void serialLoop(){
    if(Serial.available()){
        ;
    }
}

void loadData(){
    char ok[2 + 1];
    EEPROM.begin(512);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    for(uint i = 0; i<16; i++) EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) +i, aes_key[i]);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 16,ok);
    EEPROM.end();
    if (String(ok) != String("OK")){
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
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    for(uint i = 0; i<16; i++) EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) +i, aes_key[i]);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 16,ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials and other information");
    Serial.println(mqtt_server);
    Serial.println(myHostname);
    ESP.restart();
}