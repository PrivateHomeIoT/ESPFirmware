#ifndef handleHttp_h
#define handleHttp_h
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Arduino.h>

extern ESP8266WebServer server(80);
extern IPAddress serverIP(0,0,0,0);

boolean captivePortal();
void handleWifi();
void handleWifiSave();
void handleNotFound();
void handleRoot();
void httpSetup();

#endif
