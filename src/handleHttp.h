#ifndef handleHttp_h
#define handleHttp_h

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

extern ESP8266WebServer server;
extern IPAddress serverIP;

boolean captivePortal();
void handleWifi();
void handleWifiSave();
void handleNotFound();
void handleRoot();
void httpSetup();
#endif