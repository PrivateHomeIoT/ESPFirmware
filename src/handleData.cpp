#include <Arduino.h>
#include <EEPROM.h>
#include "handleData.h"
#include "handleEncryption.h"
#include "handleHttp.h"
#include "handleWifi.h"
#include "handleMQTT.h"
#include "handlePorts.h"
#include <ArduinoJson.h>

char* rawString;
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
    if(Serial.available()){
        Serial.flush();
        Serial.println("PrivateHome-ESPFirmware");
        Serial.println();
        Serial.println("Welcome to the ESPFirmware. This console is thought for experts and the automatic configuration by the server.");
        Serial.println("If you want to get further information, have a look at our GitHub-Repo and Wiki. https://github.com/PrivateHomeIoT/ESPFirmware");
        Serial.println();
    }
}

void serialEvent(){
    while(Serial.available()){
        byte inByte = Serial.read();
        char inChar = (char)inByte;
        if(inChar == '/n'){
            Serial.println("Message received: " + (String)rawString);
            if(strcmp(rawString, (const char*)"setSSID") == 0) setSSID = true;
            else if(setSSID) {
                for(uint i = 0; i<33; i++) ssid[i] = rawString[i];
                setSSID = false;
                saveData();
                Serial.println("SUCCESS");
            }
            else if(strcmp(rawString, (const char*)"setPW") == 0) setPW = true;
            else if(setPW) {
                for(uint i = 0; i<65; i++) password[i] = rawString[i];
                setPW = false;
                saveData();
                Serial.println("SUCCESS");
            }
            else if(strcmp(rawString, (const char*)"setServer") == 0) setServer = true;
            else if(setServer) {
                for(uint i = 0; i<17; i++) mqtt_server[i] = rawString[i];
                setServer = false;
                saveData();
                Serial.println("SUCCESS");
            }
            else if(strcmp(rawString, (const char*)"setAESKey") == 0) setKey = true;
            else if(setKey) {
                // decodeKEY(rawString);
                for(uint i = 0; i<sizeof(rawString); i++) aes_key[i] = rawBytes[i];
                setKey = false;
                saveData();
                Serial.println("SUCCESS");
            }
            else if(strcmp(rawString, (const char*)"setID") == 0) setID = true;
            else if(setID) {
                for(uint i = 0; i<sizeof(rawString); i++) myHostname[i] = rawString[i];
                setID = false;
                saveData();
                Serial.println("SUCCESS");
            }
            rawString = (char*)"";
            delay(1000);
        } else {
            rawString += inChar;
            rawBytes += inByte;
        }
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
    if (firstBoot) Serial.println("No data found...");
    // Serial.println("Recovered credentials:");
    // Serial.println(ssid);
    // Serial.println(strlen(password) > 0 ? "********" : "<no password>");
    if(sizeof(ssid)>0) firstBoot = false;
    // Serial.println("Finished loading data");
    // Serial.println(mqtt_server);
    // Serial.println(myHostname);
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