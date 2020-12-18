#ifndef handleMQTT_h
#define handleMQTT_h
#include <PubSubClient.h>
#include <Arduino.h>
#include "handleWifi.h"

extern char* mqtt_server;
PubSubClient client(espClient);

void loopMQTT();
void setupMQTT();

#endif
