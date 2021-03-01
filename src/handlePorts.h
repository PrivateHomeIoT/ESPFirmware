#ifndef handlePorts_h
#define handlePorts_h

#include <Arduino.h>

struct Port{
    char* id;
    bool isAnalog = false;
    bool isOutput = true;
    uint8_t port;
};

void actPort();
void setupPorts();

#endif