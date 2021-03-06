#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include "handleWifi.h"
#include "handleData.h"

char ssid[33];
char password[65];
WiFiClient espClient;
char myHostname[6] = "home1";
unsigned long lastConnectTry = 0;
bool firstBoot = false;
boolean connect;
boolean connected;

void wifiSetup(){
    Serial.println("Connecting as wifi client...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && ssid[0] != NULL) {
        delay(500);
        Serial.print(".");
    }

    int connRes = WiFi.waitForConnectResult();
    Serial.print("connRes: ");
    Serial.println(connRes);
    if(connRes == 3) connected = true;
}

void wifiLoop(){
    if (connect){
        Serial.println("Connect requested");
        connect = false;
        wifiSetup();
        lastConnectTry = millis();
    }{
        unsigned int s = WiFi.status();
        if (s == 0 && millis() > (lastConnectTry + 60000)){
            /* If WLAN disconnected and idle try to connect */
            connect = true;
        }
        if (WiFi.status() != s){ // WLAN status change
            Serial.print("Status: ");
            Serial.println(s);
            if (s == WL_CONNECTED){
                /* Just connected to WLAN */
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(ssid);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
                // Setup MDNS responder
                if (!MDNS.begin(myHostname)){
                    Serial.println("Error setting up MDNS responder!");
                } else {
                    Serial.println("mDNS responder started");
                    // Add service to MDNS-SD
                    MDNS.addService("http", "tcp", 80);
                }
            } else if (s == WL_NO_SSID_AVAIL){
                WiFi.disconnect();
            }
        } if (s == WL_CONNECTED) {
            MDNS.update();
        }
    }
}