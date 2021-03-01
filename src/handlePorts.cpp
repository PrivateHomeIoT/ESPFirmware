#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWiFi.h"

char* actPort(char* topic, char* msg){
    if (strcmp(msg, (char*)"1.0") == 0){
        digitalWrite(LED_BUILTIN, LOW);
        client.publish((char*)("Home/stat/" + (String)myHostname).c_str(), "ON");
    } else if (strcmp(msg, (char*)"0.0") == 0){
        digitalWrite(LED_BUILTIN, HIGH);
        client.publish((char*)("Home/stat/" + (String)myHostname).c_str(), "OFF");
    } else if (strcmp(msg, (char*)"ON") == 0){
        digitalWrite(LED_BUILTIN, LOW);
        client.publish((char*)("Home/stat/" + (String)myHostname).c_str(), "ON");
    } else if (strcmp(msg, (char*)"OFF") == 0){
        digitalWrite(LED_BUILTIN, HIGH);
        client.publish((char*)("Home/stat/" + (String)myHostname).c_str(), "OFF");
    }
    return msg;
}  