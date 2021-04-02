#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWiFi.h"
#include "handleData.h"

uint8_t outputs[64];
uint8_t inputs[64];

bool isOutput(uint8_t port){
    for(uint i = 0; i< sizeof(outputs); i++){
        if(outputs[i] == port) return true;
    }
    return false;
}

void actPort(char* msg){
    JsonObject data = parseJSON(msg);
    uint8_t port = (int)(data["pin"]);
    uint8_t value = (int)(data["value"]);
    if(isOutput(port)){
        if(value == 0) digitalWrite(port, LOW);
        else if(value == 1024) digitalWrite(port, HIGH);
        else analogWrite(port, value);
    }
}  

void configPorts(char* msg){
    // Serial.println("Setupdata received:");
    // Serial.println(msg);
    // JsonObject data = parseJSON(msg);
    // randomCode = data["randomCode"];
    // Serial.println("RandomCode: "+ (String)(randomCode));
    // for (int i = 0; i < 32; i++){
    //     // outputs[i] = data["outputs"][i]["pin"];
    //     // actPort(data["outputs"][i]);
    // }
}
