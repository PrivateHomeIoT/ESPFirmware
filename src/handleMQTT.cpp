#include "handleMQTT.h"
#include "handleWifi.h"
#include "handlePorts.h"
#include "Arduino.h"

char chars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','-','_'};
char mqtt_server[17] = "192.168.178.136";
int mqtt_port = 1500;
PubSubClient client(espClient);

Port getPortOfID(char p){
  for(uint i = 0; i < sizeof(chars); i++) if(p == ports[i].identifier) return ports[i];
  return Port(p);
}

void connectMQTT() {
  // Loop until we're reconnected
  if(connected){
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_server);
    // Attempt to connect
    if (client.connect(myHostname)) {
      Serial.println("connected");
      // Once connected, publish an announcement
      client.publish((char*)((String)myHostname + "/status").c_str(), "online");
      client.publish((char*)("Home/stat/" + (String)myHostname).c_str(), "1.0");
      client.subscribe(((char*)((String)myHostname + "/").c_str()));
      for (uint i = 0; i < sizeof(chars); i++) client.subscribe((char*)("Home/config/" + (String)myHostname + chars[i]).c_str());
      for (uint i = 0; i < sizeof(chars); i++) client.subscribe((char*)("Home/switch/cmnd/" + (String)myHostname + chars[i]).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char *msg;
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i]=(char)payload[i];
  }
  for(uint i = 0; i < sizeof(topic); i++) if(strcmp(topic, (char*)("Home/switch/cmnd/" + (String)myHostname + chars[i]).c_str()) == 0) actPort(getPortOfID(chars[i]), msg);
  for(uint i = 0; i < sizeof(topic); i++) if(strcmp(topic, (char*)("Home/config/" + (String)myHostname + chars[i]).c_str()) == 0) configPort(getPortOfID(chars[i]), msg);
}

void loopMQTT() {
  if (!client.connected()) connectMQTT();
  client.loop();
}

void setupMQTT(){
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
    client.publish((char*)((String)myHostname + "/config").c_str(), myHostname);
}