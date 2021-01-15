#include "handleMQTT.h"
#include "handleWifi.h"
#include "handleEncryption.h"
#include "handlePorts.h"

char* mqtt_server = (char*) "private.local";
int mqtt_port = 1880;
char* msg;
PubSubClient client(espClient);

void connectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(myHostname)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(((String)myHostname + "/1").c_str(), "online");
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
  msg = decrypt((char*)(payload));
  topic = decrypt(topic);
  client.publish(encrypt(topic), encrypt(handlePort(msg, topic)));
}

void loopMQTT() {
  if (!client.connected()) connectMQTT();
  client.loop();
}

void setupMQTT(){
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
    client.publish(encrypt((char*)((String)myHostname + "/config").c_str()), myHostname);
}