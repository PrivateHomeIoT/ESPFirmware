#include "handleHttp.h"
#include "handleJSON.h"
#include "handleMQTT.h"
#include "handleWifi.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "handleEncryption.h"
#include "configuration.h"
=======

#define BLOCK_SIZE 16
>>>>>>> parent of c80bff1... Outsourced Ports and Encryption
=======

#define BLOCK_SIZE 16
>>>>>>> parent of c80bff1... Outsourced Ports and Encryption

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