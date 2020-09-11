#include <ESP8266WiFi.h>
#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

StaticJsonBuffer<200> jsonBuffer;

const char* ssid;
const char* password;
JsonObject& ports = jsonBuffer.parseObject("{}");

bool firstBoot(){
    SPIFFS.begin();
    File s = SPIFFS.open("/ssid.txt", "r");
    if (s){
        ssid = s.readString().c_str();
        File p = SPIFFS.open("/pass.txt", "r");
        if (p) password = s.readString().c_str();
        return false;
        }
    File g = SPIFFS.open("/ports.txt", "r");
    if (s) ports = jsonBuffer.parseObject(g.readString());
    return true;
}

void showCaptive(){

}

void wifiSetup(){
    if (firstBoot) showCaptive();
    else if (SPIFFS.open("/wifiMode.txt", "r").readString().equals("Captive")) showCaptive();
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