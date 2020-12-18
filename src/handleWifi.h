#ifndef handleWifi_h
#define handleWifi_h
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Arduino.h>

extern char ssid[33];
extern char password[65];
IPAddress apIP(172, 217, 28, 1);   
IPAddress netMsk(255, 255, 255, 0);
extern WiFiClient espClient;
extern DNSServer dnsServer;
extern const char *softAP_ssid;
extern const char *softAP_password;
extern const char *myHostname;

void connectWifi();
void wifiSetup();
void wifiLoop();

#endif