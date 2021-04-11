#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Arduino.h>
#include "handleHttp.h"
#include "handleWifi.h"
#include "handleData.h"
#include "handleMQTT.h"
#include "handleEncryption.h"

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void httpSetup(){
  server.on("/", handleWifi);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleWifi); //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleWifi);       //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");
}

boolean isIp(String str){
  for (size_t i = 0; i < str.length(); i++){
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')){
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip){
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

boolean captivePortal(){
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")){
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop();             // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleWifi(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  String Page;
  Page += F(
      "<!DOCTYPE html><html lang='en'><head>"
      "<meta name='viewport' content='width=device-width'>"
      "<title>PrivateIoT - Setup</title></head><body>"
      "<h1>Wifi config</h1>");
  if (server.client().localIP() == apIP){
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else{
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  Page +=
      String(F(
          "\r\n<br />"
          "<table><tr><th align='left'>SoftAP config</th></tr>"
          "<tr><td>SSID ")) +
      String(softAP_ssid) +
      F("</td></tr>"
        "<tr><td>IP ") +
      toStringIp(WiFi.softAPIP()) +
      F("</td></tr>"
        "</table>"
        "\r\n<br />"
        "<table><tr><th align='left'>WLAN config</th></tr>"
        "<tr><td>SSID ") +
      String(ssid) +
      F("</td></tr>"
        "<tr><td>IP ") +
      toStringIp(WiFi.localIP()) +
      F("</td></tr>"
        "</table>"
        "\r\n<br />"
        "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0){
    for (int i = 0; i < n; i++){
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  }
  else{
    Page += F("<tr><td>No WLAN found</td></tr>");
  }
  Page += F(
      "</table>"
      "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
      "<input type='text' placeholder='network' name='n'/>"
      "<br /><input type='password' placeholder='password' name='p'/>"
      "<br /><input type='text' placeholder='IP adress of the PrivateHome-Server' name='s'/>"
      "<br /><input type='text' placeholder='Device ID' name='id'/>"
      "<br /><input type='text' placeholder='Encryption-Key' name='key'/>"
      "<br /><input type='submit' value='Connect/Disconnect'/></form>"
      "</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

void handleWifiSave(){
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.arg("s").toCharArray((char*)mqtt_server, sizeof(mqtt_server) - 1);
  server.arg("id").toCharArray((char*)myHostname, sizeof(myHostname)-1);
  char keyRaw[16];
  server.arg("key").toCharArray(keyRaw, 16);
  decodeKEY(keyRaw);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop();             // Stop is needed because we sent no content length
  saveData();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound(){
  if (captivePortal()){ // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++){
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}