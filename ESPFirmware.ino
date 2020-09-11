#include <ESP8266WiFi.h>
#include "FS.h"
#include <jsonlib.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

const char* ssid;
const char* password;

struct Port{
  char pin;
  char type = OUTPUT;

  Port(char pin, char type);
};

Port configuredPorts[] = {};

bool firstBoot(){
    SPIFFS.begin();
    File s = SPIFFS.open("/ssid.txt", "r");
    if (s){
        ssid = s.readString().c_str();
        File p = SPIFFS.open("/pass.txt", "r");
        if (p) password = s.readString().c_str();
        File g = SPIFFS.open("/ports.txt", "r");
        String ports = jsonExtract(g.readString(),"ports");
        for (int i = 0; i<ports.length(); i++) configuredPorts[i] = new Port(jsonExtract(ports, "port"),jsonExtract(ports, "function"));
        return false;
        }
    return true;
}

void showCaptive(){

}

void wifiSetup(){
    if (firstBoot) showCaptive();
    else if (SPIFFS.open("/wifiMode.txt", "r").readString().equals("captive")) showCaptive();
    else {
        WiFi.begin();
        Serial.println("");

        // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void setup(){
    Serial.begin(115200);
    wifiSetup();
}

void loop(){
    Serial.print("hi");
}