#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWiFi.h"

Port ports[64];

void setupPorts(){
    for(uint i=0; i<64; i++) ports[i] = Port(chars[i]);
}

void actPort(Port port, char* msg){
    if(!port.isAnalog && port.isOutput){
        if (strcmp(msg, (char*)"1.0") == 0){
            digitalWrite(port.hardwarePort, LOW);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)port.identifier).c_str(), "ON");
        } else if (strcmp(msg, (char*)"0.0") == 0){
            digitalWrite(port.hardwarePort, HIGH);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)port.identifier).c_str(), "OFF");
        } else if (strcmp(msg, (char*)"ON") == 0){
            digitalWrite(port.hardwarePort, LOW);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)port.identifier).c_str(), "ON");
        } else if (strcmp(msg, (char*)"OFF") == 0){
            digitalWrite(port.hardwarePort, HIGH);
            client.publish((char*)("Home/stat/" + (String)myHostname + (String)port.identifier).c_str(), "OFF");
        }
    } else if(port.isAnalog && port.isOutput){
        int value = 1024 * (int(msg[0]) + int(msg[2])/10 + int(msg[3])/100);
        analogWrite(port.hardwarePort, value);
    }
}  

void configPort(Port portNumber, char* msg){
}
