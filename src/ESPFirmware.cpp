#include "handleHttp.h"
#include "handleJSON.h"
#include "handleMQTT.h"
#include "handleWifi.h"
<<<<<<< HEAD

#define BLOCK_SIZE 16
=======
#include "handleEncryption.h"
>>>>>>> parent of 7cf6583... implemented configuration.h

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