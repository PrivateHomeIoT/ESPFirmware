# 1 "/var/folders/jy/6hxgl9194dq1x7srbw56br_80000gn/T/tmpy5dmhfp2"
#include <Arduino.h>
# 1 "/Users/maximilian/Documents/Projekte/PrivateHomeIot/ESPFirmware/src/ESPFirmware.ino"
#include "handleHttp.h"
#include "jsonHandler.h"
#include "handleMQTT.h"
#include "handleWifi.h"

#define BLOCK_SIZE 16

char *key;
bool firstBoot = false;
boolean connect;
void setup();
void loop();
#line 12 "/Users/maximilian/Documents/Projekte/PrivateHomeIot/ESPFirmware/src/ESPFirmware.ino"
void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    wifiSetup();
    httpSetup();
}

void loop()
{
    wifiLoop();
}