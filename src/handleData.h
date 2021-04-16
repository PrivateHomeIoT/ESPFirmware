#ifndef handleData_h
#define handleData_h

#include <ArduinoJson.h>

extern char ssid[33];
extern char password[65];
extern bool firstBoot;

JsonObject parseJSON(char* rawJSON);
void serialSetup();
void serialEvent();
void serialLoop();
void saveData();
void loadData();

#endif