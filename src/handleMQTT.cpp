#include "handleMQTT.h"
#include "handleWifi.h"
#include "handlePorts.h"
#include "handleEncryption.h"
#include "Arduino.h"

char mqtt_server[17] = "192.168.178.136";
int mqtt_port = 1500;
String randomCode = "";
PubSubClient client(espClient);

void connectMQTT() {
  // Loop until we're reconnected
  if(connected){
    if (!client.connected()) {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_server);
    // Attempt to connect
    if (client.connect(myHostname)) {
      Serial.println("connected");
      String hostname;
      for(uint i = 0; i<5; i++) hostname += myHostname[i]; 
      Serial.println(hostname);
      client.publish((char*)("home/setupRequest/" + hostname).c_str(), encryptFromChar(myHostname,6));
      Serial.println(((char*)("home/setup/" + hostname).c_str()));
      client.subscribe(((char*)("home/setup/") + hostname).c_str());
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

void callback(char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  printArray(payload,length);
  uint8_t iv[16];
  uint8_t msg[length-16];
  for (unsigned int i = 0; i < length; i++){
    if(i<16) iv[i] = payload[i];
    else msg[i-16] = payload[i];
  }
  String decrypted = decryptToChar(msg, iv, length-16);
  Serial.println(decrypted);
  if(strcmp(topic, (char*)("home/switch/cmd/" + randomCode).c_str()) == 0) actPort((char*)decrypted.c_str());
  if(strcmp(topic, (char*)("home/setup/" + (String)myHostname).c_str()) == 0) configPorts((char*)decrypted.c_str(), decrypted.length());
}

void loopMQTT() {
  if (!client.connected()) connectMQTT();
  client.loop();
}

void setupMQTT(){
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
}