#include "handlePorts.h"

Function functions[] {Function(), Function((char*)"Input")};
Port* configuredPorts;

int getPosition(Function p){
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

char* handlePort (char* msg, char* topic){
    Port actual = Port(-1,-1,(char*)"x");
    for(int i=0; i<sizeof(configuredPorts); i++){
        if(configuredPorts[i].topic == topic){
            actual = configuredPorts[i];
            break;
        }
    }
    if (!(actual.type.isAnalog) && !(actual.type.isPWM) && actual.type.pinMode == (char*)"OUTPUT"){
        if (msg == actual.type.mqttCmds[1]) digitalWrite(actual.pin,HIGH);
        else if (msg == actual.type.mqttCmds[0]) digitalWrite(actual.pin,LOW);
        return msg;
    } else return "Function not defined";
}