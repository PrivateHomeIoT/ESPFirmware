#include "handleMQTT.h"
#include "handleWifi.h"
<<<<<<< HEAD
#include "handleEncryption.h"
#include "configuration.h"
=======
>>>>>>> parent of c80bff1... Outsourced Ports and Encryption

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
<<<<<<< HEAD
=======
//char* topic;
String clientId;
PubSubClient client(espClient);
>>>>>>> parent of c80bff1... Outsourced Ports and Encryption

void connectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // clientId = "ESP8266Client-"; //TODO: usage of MQTT ID
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    // if (client.connect(clientId.c_str())) {
    if (client.connect(myHostname)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(myHostname, "online");
      // ... and resubscribe
      client.subscribe(myHostname);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void loopMQTT() {
  if (!client.connected()) connectMQTT();
  client.loop();
}

void setupMQTT(){
    client.setServer(serverIP.toString().c_str(), mqtt_port);
    client.setCallback(callback);
    connectMQTT();
}