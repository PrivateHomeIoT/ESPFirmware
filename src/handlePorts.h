#ifndef handlePorts_h
#define handlePorts_h

#include <Arduino.h>
extern uint8_t outputs[64];
extern uint8_t inputs[64];

void actPort(char* msg, uint length);
void configPorts(char* msg, uint length);

#endif