#ifndef configuration_h
#define configuration_h
    #include <Arduino.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    #include <DNSServer.h>
    #include <WiFiClient.h>
    #include <PubSubClient.h>

    ESP8266WebServer server(80);            //internal webserver
    IPAddress serverIP(0,0,0,0);            //IP of the PrivateHome-Server
    WiFiClient espClient;                   //internal WiFiclient
    DNSServer dnsServer;                    //internal DNS-server
    IPAddress apIP(172, 217, 28, 1);        //IP-address of the ESP in the configuration AP
    IPAddress netMsk(255, 255, 255, 0);     //subnetmask of the configuration AP
    PubSubClient client(espClient);         //internal MQTT-client

    #ifndef APSSID
    #define APSSID "PrivateIoTSETUP"        //SSID of the configuration AP
    #define APPSK "privatehome"             //password of the configuration AP
    #endif

    const char *softAP_ssid = APSSID;       // ^^
    const char *softAP_password = APPSK;    // ^^
    const char *myHostname = "espDefault";  //hostname of the ESP --> later the id provided by the PrivateHome-server
    const byte DNS_PORT = 53;               //DNS-port (internal)
    char ssid[33] = "";                     //SSID of the home/IoT WiFi --> later configured in the Web GUI
    char password[65] = "";                 //password of the home/IoT WiFi --> later configured in the Web GUI
    char *key;                              //encryption key for secure MQTT encryption
    int mqtt_port = 1880;                   //port of the MQTT-broker
    
#endif