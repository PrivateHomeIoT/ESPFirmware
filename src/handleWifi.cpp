#include <WiFiClient.h>
#include "handleHttp.h"
#include "handleWifi.h"
#include "handleJSON.h"
#include "configuration.h"

unsigned long lastConnectTry = 0;
bool firstBoot = false;
boolean connect;

void connectWifi()
{
    Serial.println("Connecting as wifi client...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int connRes = WiFi.waitForConnectResult();
    Serial.print("connRes: ");
    Serial.println(connRes);
}

void wifiSetup()
{
    Serial.println("Configuring access point...");
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(softAP_ssid, softAP_password);
    delay(500); // Without delay I've seen the IP address blank
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    loadData(); // Load WLAN credentials from network
    connect = strlen(ssid) > 0;
}

void wifiLoop()
{
    if (connect)
    {
        Serial.println("Connect requested");
        connect = false;
        connectWifi();
        lastConnectTry = millis();
    }
    {
        unsigned int s = WiFi.status();
        if (s == 0 && millis() > (lastConnectTry + 60000))
        {
            /* If WLAN disconnected and idle try to connect */
            /* Don't set retry time too low as retry interfere the softAP operation */
            connect = true;
        }
        if (WiFi.status() != s)
        { // WLAN status change
            Serial.print("Status: ");
            Serial.println(s);
            if (s == WL_CONNECTED)
            {
                /* Just connected to WLAN */
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(ssid);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());

                // Setup MDNS responder
                if (!MDNS.begin(myHostname))
                {
                    Serial.println("Error setting up MDNS responder!");
                }
                else
                {
                    Serial.println("mDNS responder started");
                    // Add service to MDNS-SD
                    MDNS.addService("http", "tcp", 80);
                }
            }
            else if (s == WL_NO_SSID_AVAIL)
            {
                WiFi.disconnect();
            }
        }
        if (s == WL_CONNECTED)
        {
            MDNS.update();
        }
    }
    // Do work:
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
}