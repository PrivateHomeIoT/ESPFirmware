#include "handlePorts.h"
//#include "handleEncryption.h"

Function functions[] {Function(), Function((char*)"Input")};
Port* configuredPorts;

int getPosition(Function p)
{
    // for (int i = 0; i < sizeof(functions); i++)
    for (int i = 0; i < 2; i++)
    {
        if (p.pinMode == functions[i].pinMode && p.isAnalog == functions[i].isAnalog && p.isPWM == functions[i].isPWM && p.values == functions[i].values && p.mqttCmds == functions[i].mqttCmds)
        {
            return i;
        }
    }
    return 0;
}