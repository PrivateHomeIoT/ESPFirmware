#include <Arduino.h>
#include <EEPROM.h>
#include "handleData.h"
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

void loadData(){
    EEPROM.begin(512);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    char ok[2 + 1];
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    bool isFirst = true;
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 3* sizeof(ports)+ sizeof(ok),isFirst);
    if(!isFirst){
        for(uint i = 0; i < sizeof(ports); i++){
            EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + i, ports[i].hardwarePort);
            EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + sizeof(ports) + i, ports[i].isAnalog);
            EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 2* sizeof(ports) + i, ports[i].isOutput);
        }
    }
    EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 3* sizeof(ports),ok);
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
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), myHostname);
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname), mqtt_server);
    bool isFirst = true;
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 3* sizeof(ports)+ sizeof(ok),isFirst);
    for(uint i = 0; i < sizeof(ports); i++){
        EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + i, ports[i].hardwarePort);
        EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + sizeof(ports) + i, ports[i].isAnalog);
        EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 2* sizeof(ports) + i, ports[i].isOutput);
    }
    EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(myHostname) + sizeof(mqtt_server) + 3* sizeof(ports),ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials and other information");
    Serial.println(mqtt_server);
    Serial.println(myHostname);
    ESP.restart();
}