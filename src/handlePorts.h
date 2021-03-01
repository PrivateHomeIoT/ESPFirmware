#ifndef handlePorts_h
#define handlePorts_h

#include <Arduino.h>

struct Port{
    bool isAnalog = false;
    bool isOutput = true;
};

extern Port ports[64];

void actPort(int portNumber, char* msg);
void setupPorts();

#endif