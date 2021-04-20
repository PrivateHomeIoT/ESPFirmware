#ifndef handleWifi_h
#define handleWifi_h

#include <WiFiClient.h>
#include <ESP8266mDNS.h>

extern char ssid[33];
extern char password[65];
extern char myHostname[6];
extern bool firstBoot;
extern boolean connect;
extern boolean connected;

extern WiFiClient espClient;

void wifiSetup();
void wifiLoop();
#endif