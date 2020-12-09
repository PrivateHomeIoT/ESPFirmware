#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "handleHttp.h"
#include "jsonHandler.h"

#ifndef APSSID
#define APSSID "PrivateIoTSETUP"
#define APPSK  "privatehome"
#endif

char ssid[33] = "";
char password[65] = "";
bool firstBoot = false;
boolean connect;

DNSServer dnsServer;
ESP8266WebServer server(80);
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;
const char *myHostname = "esp8266";
const byte DNS_PORT = 53;
unsigned long lastConnectTry = 0;

// const char *softAP_ssid = APSSID;
// const char *softAP_password = APPSK;
// const char *myHostname = "esp8266";
// const byte DNS_PORT = 53;
// unsigned long lastConnectTry = 0;

//Port configuredPorts[] = {};

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
  Serial.println(connRes);
}

void setup(){
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.println("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(softAP_ssid, softAP_password);
    delay(500); // Without delay I've seen the IP address blank
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
    server.on("/", handleRoot);
    server.on("/wifi", handleWifi);
    server.on("/wifisave", handleWifiSave);
    server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
    server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
    server.onNotFound(handleNotFound);
    server.begin(); // Web server start
    Serial.println("HTTP server started");
    loadData(); // Load WLAN credentials from network
    connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
}

void loop(){
    if (connect) {
        Serial.println("Connect requested");
        connect = false;
        connectWifi();
        lastConnectTry = millis();
    }
    {
        unsigned int s = WiFi.status();
        if (s == 0 && millis() > (lastConnectTry + 60000)) {
            /* If WLAN disconnected and idle try to connect */
            /* Don't set retry time too low as retry interfere the softAP operation */
            connect = true;
            }
        if (WiFi.status() != s) { // WLAN status change
            Serial.print("Status: ");
            Serial.println(s);
            int status = s;
            if (s == WL_CONNECTED) {
                /* Just connected to WLAN */
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(ssid);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());

                // Setup MDNS responder
                if (!MDNS.begin(myHostname)) {
                Serial.println("Error setting up MDNS responder!");
                } else {
                Serial.println("mDNS responder started");
                // Add service to MDNS-SD
                MDNS.addService("http", "tcp", 80);
                }
        } else if (s == WL_NO_SSID_AVAIL) {
            WiFi.disconnect();
            }
        }
        if (s == WL_CONNECTED) {
        MDNS.update();
        }
    }
    // Do work:
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
}