#ifndef handleWifi_h
#define handleWifi_h

#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

extern char ssid[33];
extern char password[65];
extern const char *softAP_ssid;
extern const char *softAP_password;
extern const char *myHostname;
extern bool firstBoot;
extern boolean connect;

extern IPAddress apIP;   
extern IPAddress netMsk;
extern WiFiClient espClient;
extern DNSServer dnsServer;

void connectWifi();
void wifiSetup();
void wifiLoop();
#endif