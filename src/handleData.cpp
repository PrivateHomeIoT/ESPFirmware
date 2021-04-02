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
    String input = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    deserializeJson(doc, input);
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
        if(Serial.readStringUntil(':')=="iv"){
            
        }
    }
}

void loadData(){
    EEPROM.begin(512);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    //EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server), aes_key);
    //EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + sizeof(aes_key), ok);
    EEPROM.end();
    // char ok[2 + 1];
    // if (String(ok) != String("OK")){
    //     ssid[0] = 0;
    //     password[0] = 0;
    //     firstBoot = true;
    // }
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
    //EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server), aes_key);
    // char ok[2 + 1] = "OK";
    //EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + sizeof(aes_key),ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials and other information");
    Serial.println(mqtt_server);
    Serial.println(myHostname);
    ESP.restart();
}