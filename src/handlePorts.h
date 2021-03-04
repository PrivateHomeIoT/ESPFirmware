#ifndef handlePorts_h
#define handlePorts_h

#include <Arduino.h>

struct Port{
    char identifier;
    uint8_t hardwarePort = 2;
    bool isAnalog = false;
    bool isOutput = true;

    Port(){
        identifier = '#';
    }

    Port(char id){
        identifier = id;
    }

    Port(char id, uint8_t port){
        identifier = id;
        hardwarePort = port;
    }

    Port(char id, uint8_t port, bool analog, bool output){
        identifier = id;
        hardwarePort = port;
        isAnalog = analog;
        isOutput = output;
    }
};

extern Port ports[64];
extern char chars[];

Port getPortofID(char p);
int getArrayIDofPort(Port port);
void setupPorts();
void actPort(Port port, char* msg);
void configPort(Port port, char* msg);

#endif