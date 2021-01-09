#ifndef handleMQTT_h
#define handleMQTT_h

#include <PubSubClient.h>
#include <Arduino.h>

extern PubSubClient client;

void loopMQTT();
void setupMQTT();

#endif