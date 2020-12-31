#include "handleHttp.h"
#include "handleJSON.h"
#include "handleMQTT.h"
#include "handleWifi.h"
#include "handleEncryption.h"

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