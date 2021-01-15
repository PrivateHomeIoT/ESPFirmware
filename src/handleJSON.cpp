#include <Arduino.h>
#include <Arduino_Json.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include "handleJSON.h"
#include "handlePorts.h"
#include "handleHttp.h"

void parsePorts(String rawJSON){
    JSONVar ports = JSON.parse(rawJSON)["ports"];
    int next = 0;
    for (int i = 0; i < 64; i++)
    {
        if (ports[i]["port"] != "")
        {
            configuredPorts[next].pin = (int) ports[i]["port"];
            configuredPorts[next].type = functions[int(ports[i]["function"])];
            configuredPorts[next].topic = (char*) JSON.stringify(ports[i]["topic"]).c_str();
            next++;
        }
    }
}

String encodePorts(){
    JSONVar ports;
    ports[0]["port"] = 1;
    ports[0]["function"] = 1;
    for (int i = 0; i < 64; i++)
    {
        ports[i]["port"] = configuredPorts[i].pin;
        ports[i]["function"] = getPosition(configuredPorts[i].type);
        ports[i]["topic"] = configuredPorts[i].topic;
    }
    return JSON.stringify(ports);
}

void loadData(){
    EEPROM.begin(512);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    char ok[2 + 1];
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
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
    LittleFS.begin();
    File g = LittleFS.open("/ports.txt", "r");
    if (g)
        parsePorts(g.readString());
    Serial.println("Finished loading data");
}

void saveData(){
    EEPROM.begin(512);
    EEPROM.put(0, ssid);
    EEPROM.put(0 + sizeof(ssid), password);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials");
}