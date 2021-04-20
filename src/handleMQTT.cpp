#include "handleMQTT.h"
#include "handleWifi.h"
#include "handlePorts.h"
#include "handleEncryption.h"
#include "Arduino.h"

char mqtt_server[17] = "192.168.178.136";
int mqtt_port = 1500;
const char* randomCode = (char*)"1234567890";
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
      client.publish((char*)("home/setupRequest/" + (String)myHostname).c_str(), encryptFromChar(myHostname));
      client.publish((char*)("home/status/" + (String)myHostname).c_str(), encryptFromChar((char*)"online"));
      //Serial.println("Published first MQTT-message: "+ (String)encrypt((char*)"online"));
      client.subscribe(((char*)("home/setup/" + (String)myHostname).c_str()));
      client.subscribe((char*)("home/switch/cmd/" + (String)randomCode).c_str());
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
  char *msg = (char*)"";
  char* iv = (char*)"";

  for (unsigned int i = 0; i < length; i++) {
    if(i<16) iv[i] = payload[i];
    else msg[i-16] = (char)payload[i];
  }
  char* decrypted = decryptToChar(msg,iv);

  Serial.println(decrypted);
  if(strcmp(topic, (char*)("home/switch/cmd/" + (String)randomCode).c_str()) == 0) actPort(decrypted);
  if(strcmp(topic, (char*)("home/setup/" + (String)myHostname).c_str()) == 0) configPorts(decrypted);
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