#include "handleWifi.h"
#include "handleHttp.h"
#include "handleJSON.h"
#include "handleMQTT.h"
#include "Arduino.h"

void setup(){
    delay(1000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    Serial.println();
    loadData();
    wifiSetup();
    httpSetup();
    delay(500);
}

void loop(){
    delay(100);
    wifiLoop();
    loopMQTT();
}