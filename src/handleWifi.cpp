#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include "handleWifi.h"
#include "handleData.h"

char ssid[33] = "";
char password[65] = "";
WiFiClient espClient;
char myHostname[16] = "PrivateHomeIoTE";
unsigned long lastConnectTry = 0;
bool firstBoot = false;
boolean connect;
boolean connected;

void connectWifi(){
    Serial.println("Connecting as wifi client...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int connRes = WiFi.waitForConnectResult();
    Serial.print("connRes: ");
    Serial.println(connRes);
    if(connRes == 3) connected = true;
}

void wifiSetup(){
    connectWifi();
}

void wifiLoop(){
    if (connect){
        Serial.println("Connect requested");
        connect = false;
        connectWifi();
        lastConnectTry = millis();
    }{
        unsigned int s = WiFi.status();
        if (s == 0 && millis() > (lastConnectTry + 60000)){
            /* If WLAN disconnected and idle try to connect */
            /* Don't set retry time too low as retry interfere the softAP operation */
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