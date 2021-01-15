#ifndef handlePorts_h
#define handlePorts_h

#include <Arduino.h>

struct Function {
    char *pinMode;
    bool isAnalog;
    bool isPWM;
    int values[20];
    char* mqttCmds[20];

    Function()
    {
        pinMode = (char*) "OUTPUT";
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = (char*)"0";
        mqttCmds[1] = (char*)"1";
    };

    Function(char *pinMode)
    {
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = (char*)"0";
        mqttCmds[1] = (char*)"1";
    };

    Function(char *pinMode, bool isAnalog, bool isPWM, int values[], char mqttCmds[]);
};

extern Function functions[];

struct Port {
    int pin;
    Function type;
    char* topic;

    Port(int port, uint function, char* mqtt_topic)
    {
        pin = port;
        type = functions[function];
        topic = mqtt_topic;
    };
};

extern Port* configuredPorts;

int getPosition(Function p);

char* handlePort(char* msg, char* topic);

#endif