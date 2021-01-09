#include "handleWifi.h"
#include "handleHttp.h"
#include <EasyOta.h>

void setup(){
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    wifiSetup();
    httpSetup();
    EasyOta.setup();
}

void loop(){
    wifiLoop();
    EasyOta.checkForUpload();
}