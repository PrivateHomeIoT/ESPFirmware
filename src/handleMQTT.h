#ifndef handleMQTT_h
#define handleMQTT_h

#include <PubSubClient.h>
#include <Arduino.h>

extern PubSubClient client;
extern char mqtt_server[17];

void loopMQTT();
void setupMQTT();

#endif