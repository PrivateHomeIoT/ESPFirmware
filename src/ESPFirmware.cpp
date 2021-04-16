#include "handleWifi.h"
#include "handleData.h"
#include "handleMQTT.h"
#include "handlePorts.h"
#include "Arduino.h"

void setup(){
    serialSetup();
    loadData();
    wifiSetup();
    delay(500);
}

void loop(){
    delay(100);
    wifiLoop();
    loopMQTT();
    serialLoop();
}