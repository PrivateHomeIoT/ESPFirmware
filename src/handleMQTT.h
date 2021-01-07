#ifndef handleMQTT_h
#define handleMQTT_h

#include <PubSubClient.h>

extern PubSubClient client;

void loopMQTT();
void setupMQTT();
#endif