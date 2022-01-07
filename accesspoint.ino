#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>


void setup_wifi(boolean reset) {  
  WiFiManager wifiManager;

  // to reset the wifiManager and the EEPROM to input new information
  if(reset){
    wifiManager.resetSettings();
  }
  
  // fetches ssid and pass from eeprom and tries to connect
  wifiManager.autoConnect("Bedroom-LED-AP", "accesspoint");

  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
}
