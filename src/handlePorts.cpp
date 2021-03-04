#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWiFi.h"
#include "handleData.h"

Port ports[64];
char chars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','-','_'};

Port getPortOfID(char p){
  for(uint i = 0; i < sizeof(chars); i++) if(p == ports[i].identifier) return ports[i];
  return Port(p);
}

int getArrayIDofPort(Port port){
    for(uint i = 0; i<64; i++) if(ports[i].identifier == port.identifier) return i;
    return -1;
}

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

void configPort(Port port, char* msg){
    JsonObject data = parseJSON(msg);
    port.hardwarePort = data["port"].as<uint8_t>();
    port.isOutput = data["output"].as<bool>();
    port.isAnalog = data["analog"].as<bool>();
    ports[getArrayIDofPort(port)] = port;
}
