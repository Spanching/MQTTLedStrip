#include <Arduino.h>
#include "fauxmoESP.h"

#define COLOR_COUNT 6

fauxmoESP fauxmo;
char* char_names[COLOR_COUNT];

void setup_fauxmo(){
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    
    String names[COLOR_COUNT] = {name, name+" in grün", name+" in rot", name+" in blau", name+" in gelb", name+" in Regenbogen"};
    
    fauxmo.enable(true);


    for(int i = 0; i < COLOR_COUNT; i++){
      char_names[i] = (char*) malloc(sizeof(char)*50);  
    }
    // Add virtual devices
    char * char_name = (char*) malloc(sizeof(char)*50);
    for(int i = 0; i < COLOR_COUNT; i++){
        memset(char_name, '\0', 50);
        names[i].toCharArray(char_name,50); 
        strcpy(char_names[i], char_name);  
        fauxmo.addDevice(char_name);
    }
    Serial.println("f");
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        for(int i = 0; i < COLOR_COUNT; i++){
            if (strcmp(char_names[i], device_name)==0) {
              if(state){
                setStripColor(i, value);  
              }else{
                setStripColor(10, value);
              }
            }
        }
    });
}
void loop_fauxmo(){
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();
}
