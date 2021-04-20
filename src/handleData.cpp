#include <Arduino.h>
#include <EEPROM.h>
#include "handleData.h"
#include "handleEncryption.h"
#include "handleWifi.h"
#include "handleMQTT.h"
#include "handlePorts.h"
#include <ArduinoJson.h>

String rawString;
byte* rawBytes;
bool setSSID = false;
bool setPW = false;
bool setServer = false;
bool setKey = false;
bool setID = false;

JsonObject parseJSON(char* rawJson){
    DynamicJsonDocument doc(1024);
    deserializeJson(doc,rawJson);
    JsonObject obj = doc.as<JsonObject>();
    return obj;
}

void serialSetup(){
    Serial.begin(9600);
    Serial.flush();
    Serial.println("PrivateHome-ESPFirmware");
    Serial.println();
    Serial.println("Welcome to the ESPFirmware. This console is thought for experts and the automatic configuration by the server.");
    Serial.println("If you want to get further information, have a look at our GitHub-Repo and Wiki. https://github.com/PrivateHomeIoT/ESPFirmware");
    Serial.println();
}

void serialLoop(){
    if(Serial.available() > 0){
        rawString = "";
        rawString = Serial.readStringUntil((char)'/');
        rawString.remove(rawString.length()-1);
        if(rawString == "test") Serial.println("hallo");
        Serial.println("Received message: " + rawString);
        if(setSSID) {
            for(uint i = 0; i<33; i++) ssid[i] = rawString[i];
            setSSID = false;
            saveData();
            Serial.println("SUCCESS");
            }
        if(setPW) {
            for(uint i = 0; i<65; i++) password[i] = rawString[i];
            setPW = false;
            saveData();
            Serial.println("SUCCESS");
            }
        if(setServer) {
            for(uint i = 0; i<17; i++) mqtt_server[i] = rawString[i];
            setServer = false;
            saveData();
            Serial.println("SUCCESS");
        }
        if(setKey) {
            aes_key[0]= (uint8_t)atoi(strtok((char*)rawString.c_str(), ","));
            Serial.println((int)aes_key[0]);
            for(uint i = 1; i<16; i++){
                aes_key[i] = (uint8_t)atoi(strtok(NULL, ","));
                Serial.println((int)aes_key[i]);
            }
            setKey = false;
            setupKey();
            saveData();
            Serial.println("SUCCESS");
            }
        if(setID) {
            for(uint i = 0; i<sizeof(rawString); i++) myHostname[i] = rawString[i];
            setID = false;
            saveData();
            Serial.println("SUCCESS");
            } 
        if(rawString == "test1") Serial.println("test2");
        else if(rawString == "setSSID") setSSID = true;
        else if(rawString == "setPW") setPW = true;
        else if(rawString == "setKey") setKey = true;
        else if(rawString == "setServer") setServer = true;
        else if(rawString == "setID") setID = true; 
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
    // Serial.println("test");
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 16,ok);
    EEPROM.end();
    if (String(ok) != String("OK")){
        ssid[0] = 0;
        password[0] = 0;
        firstBoot = true;
    }
    if (firstBoot) Serial.println("No data found...");
    if(sizeof(ssid)>0){
        firstBoot = false;
        setupKey();
    }
    setupMQTT();
    Serial.println("Finished with recovering data...");
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
}