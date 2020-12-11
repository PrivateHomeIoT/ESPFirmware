#include "handleHttp.h"
#include "jsonHandler.h"
#include "handleMQTT.h"
#include "handleWifi.h"

#define BLOCK_SIZE 16

char *key;
bool firstBoot = false;
boolean connect;

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