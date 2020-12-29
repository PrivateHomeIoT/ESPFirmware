#include <Arduino.h>
#include <Arduino_Json.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include "jsonHandler.h"
#include "handleHttp.h"
#include "ESPFirmware.ino"

struct Function
{
    char *pinMode;
    bool isAnalog;
    bool isPWM;
    int values[];
    char mqttCmds[];

    Function()
    {
        pinMode = (char*) "default";
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = 0;
        mqttCmds[1] = 1;
    };

    Function(char *pinMode)
    {
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = 0;
        mqttCmds[1] = 1;
    };

    Function(char *pinMode, bool isAnalog, bool isPWM, int values[], char mqttCmds[]);
};

Function functions[] = {Function(), Function((char*)"Input")};

struct Port
{
    int pin;
    Function type;

    Port(int port, int function)
    {
        pin = port;
        type = functions[function];
    };
};

Port configuredPorts[] = {};

int getPosition(Function p)
{
    // for (int i = 0; i < sizeof(functions); i++)
    for (int i = 0; i < 2; i++)
    {
        if (p.pinMode == functions[i].pinMode && p.isAnalog == functions[i].isAnalog && p.isPWM == functions[i].isPWM && p.values == functions[i].values && p.mqttCmds == functions[i].mqttCmds)
        {
            return i;
        }
    }
    return 0;
}

void parsePorts(String rawJSON)
{
    JSONVar ports = JSON.parse(rawJSON)["ports"];
    int next = 0;

    for (int i = 0; i < 64; i++)
    {
        if (ports[i]["port"] != "")
        {
            configuredPorts[next].pin = (int) ports[i]["port"];
            configuredPorts[next].type = functions[int(ports[i]["function"])];
            next++;
        }
    }
}

String encodePorts()
{
    JSONVar ports;
    ports[0]["port"] = 1;
    ports[0]["function"] = 1;

    for (int i = 0; i < 64; i++)
    {
        ports[i]["port"] = configuredPorts[i].pin;
        ports[i]["function"] = getPosition(configuredPorts[i].type);
    }
    return ports.stringify();
}

void loadData()
{
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

void saveData()
{
    EEPROM.begin(512);
    EEPROM.put(0, ssid);
    EEPROM.put(0 + sizeof(ssid), password);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Saved wifi credentials");
}