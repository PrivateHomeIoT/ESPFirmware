#include "handleWifi.h"
#include "handleHttp.h"
#include "handleJSON.h"
#include "handleMQTT.h"
#include "Arduino.h"

void setup(){
    delay(1000);
    Serial.begin(115200);
    loadData();
    //setupPorts(); --> handleConfig.h?
    wifiSetup();
    httpSetup();
    delay(500);
}

void loop(){
    delay(100);
    wifiLoop();
    loopMQTT();
}