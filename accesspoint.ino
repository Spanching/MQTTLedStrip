#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>

// parameters to get from AP
char * mqtt_server;
char * device_name;
char * room;

void setup_wifi(boolean reset) {
  // status led to indicate what happens
  setStat(4);  
  
  WiFiManager wifiManager;

  // to reset the wifiManager and the EEPROM to input new information
  if(reset){
    wifiManager.resetSettings();
    clearEEPROMParam();
  }

  // Adding Parameters to the WifiManager
  char* mqtt_server = (char *)malloc(32);
  if(!readMqttServer(mqtt_server)){
    mqtt_server = "192.168.2.117";
  }
  // id/device_name, placeholder/prompt, default, length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 32);
  wifiManager.addParameter(&custom_mqtt_server);

  char* device_name = (char *)malloc(32);
  if(!readName(device_name)){
    device_name = "mid-led";
  }
  WiFiManagerParameter custom_device_name("device-device_name", "device_name", device_name, 32);
  wifiManager.addParameter(&custom_device_name);

  char* room = (char *)malloc(32);
  if(!readRoom(room)){
    room = "livingroom";
  }
  WiFiManagerParameter custom_room("room", "room", room, 32);
  wifiManager.addParameter(&custom_room);
  
  // fetches ssid and pass from eeprom and tries to connect
  wifiManager.autoConnect("mid-led-AP");
  
  //Input parameters
  char* s_mqtt_server = (char*)malloc(32);
  char* s_device_name = (char*)malloc(32);
  char* s_room = (char*)malloc(32);
  strcpy(s_mqtt_server, custom_mqtt_server.getValue());
  strcpy(s_device_name, custom_device_name.getValue());
  strcpy(s_room, custom_room.getValue());
  
  //Save Parameters to EEPROM
  if(strcmp(room, s_room)!=0||strcmp(device_name, s_device_name)!=0||strcmp(mqtt_server, s_mqtt_server)!=0){
    clearEEPROMParam();
    strcpy(mqtt_server, s_mqtt_server);
    strcpy(device_name, s_device_name);
    strcpy(room, s_room);
    writeMqttServer(mqtt_server);
    writeName(device_name);
    writeRoom(room);
  }

  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
}
