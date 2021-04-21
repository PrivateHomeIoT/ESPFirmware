#include "handlePorts.h"
#include "handleMQTT.h"
#include "handleWifi.h"
#include "handleData.h"
#include "handleEncryption.h"

uint8_t outputs[64];
uint8_t inputs[64];

bool isOutput(uint8_t port){
    for(uint i = 0; i< sizeof(outputs); i++) if(outputs[i] == port) return true;
    return false;
}

void actPort(char* msg, uint length){
    JsonObject data = parseJSON(msg);
    uint8_t port = (int)(data["pin"]);
    uint16_t value = (int)(data["value"]);
    String decJSON = (String)"{\"pin\":"+(String)(uint)port+(String)",\"value\":"+(String)(uint)value+(String)"}";
    client.publish((char*)("home/status/"+ randomCode).c_str(), encryptFromChar((char*)decJSON.c_str(),decJSON.length()+1));
    if(isOutput(port)){
        if(value == (uint16_t)0) digitalWrite(port, LOW);
        else if(value == (uint16_t)1023) digitalWrite(port, HIGH);
        else analogWrite(port, value);
        Serial.println((String)"Set value of Port " + (String)port + (String)" to " + (String)value);
    }
}  

void actPort(uint8_t port, uint16_t value){
    if(isOutput(port)){
        if(value == (uint16_t)0) digitalWrite(port, LOW);
        else if(value == (uint16_t)1023) digitalWrite(port, HIGH);
        else analogWrite(port, value);
        Serial.println((String)"Set value of Port " + (String)port + (String)" to " + (String)value);
    }
}  

void configPorts(char* msg, uint length){
    Serial.println("Setupdata received:");
    Serial.println(msg);
    
    StaticJsonDocument<48> filter;
    filter["randomCode"] = true;
    filter["outputs"] = true;
    filter["inputs"] = true;
    StaticJsonDocument<192> doc;
    DeserializationError error = deserializeJson(doc, msg, length, DeserializationOption::Filter(filter));

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    const char* codeR = doc["randomCode"]; 
    randomCode = (String)codeR;
    client.subscribe((char*)("home/switch/cmd/" + randomCode).c_str());
    uint next = 0;
    for (JsonObject elem : doc["outputs"].as<JsonArray>()) {
        uint8_t pin = (uint8_t)elem["pin"]; // 0, 1
        uint16_t value = (uint8_t)elem["value"]; // 23, 23
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        actPort(pin, value);
        outputs[next] = pin;
        next += 1;
    }
    next = 0;
    for (JsonObject elem : doc["inputs"].as<JsonArray>()) {
        uint8_t pin = (uint8_t)elem["pin"]; // 0, 1
        inputs[next] = pin;
        next += 1;
    }
}
