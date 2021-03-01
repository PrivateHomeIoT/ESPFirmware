#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWiFi.h"

Port ports[64];

void actPort(int portNumber, char* msg){
    if(!ports[portNumber].isAnalog && ports[portNumber].isOutput){
        if (strcmp(msg, (char*)"1.0") == 0){
            digitalWrite(portNumber, LOW);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)portNumber).c_str(), "ON");
        } else if (strcmp(msg, (char*)"0.0") == 0){
            digitalWrite(portNumber, HIGH);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)portNumber).c_str(), "OFF");
        } else if (strcmp(msg, (char*)"ON") == 0){
            digitalWrite(portNumber, LOW);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)portNumber).c_str(), "ON");
        } else if (strcmp(msg, (char*)"OFF") == 0){
            digitalWrite(portNumber, HIGH);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)portNumber).c_str(), "OFF");
        }
    } else if(ports[portNumber].isAnalog && ports[portNumber].isOutput){
        int value = 1024 * (int(msg[0]) + int(msg[2])/10 + int(msg[3])/100);
        analogWrite(portNumber, value);
    }
}  