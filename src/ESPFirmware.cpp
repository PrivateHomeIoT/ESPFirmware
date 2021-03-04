#include "handleWifi.h"
#include "handleHttp.h"
#include "handleData.h"
#include "handleMQTT.h"
#include "handlePorts.h"
#include "Arduino.h"

void setup(){
    delay(1000);
    Serial.begin(115200);
    setupPorts();
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