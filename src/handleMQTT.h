#ifndef handleMQTT_h
#define handleMQTT_h

#include <PubSubClient.h>

// PubSubClient client(espClient);

extern PubSubClient client;

void loopMQTT();
void setupMQTT();
#endif