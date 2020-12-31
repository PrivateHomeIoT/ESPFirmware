#ifndef handlePorts_h
#define handlePorts_h

struct Function
{
    char *pinMode;
    bool isAnalog;
    bool isPWM;
    int values[20];
    char mqttCmds[20];

    Function()
    {
        pinMode = (char*) "default";
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = 0;
        mqttCmds[1] = 1;
    };

    Function(char *pinMode)
    {
        isPWM = false;
        isAnalog = false;
        values[0] = 0;
        values[1] = 1;
        mqttCmds[0] = 0;
        mqttCmds[1] = 1;
    };

    Function(char *pinMode, bool isAnalog, bool isPWM, int values[], char mqttCmds[]);
};

extern Function functions[];

struct Port
{
    int pin;
    Function type;

    Port(int port, int function)
    {
        pin = port;
        type = functions[function];
    };
};

extern Port* configuredPorts;

int getPosition(Function p);

#endif
