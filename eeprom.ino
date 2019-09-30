#include <EEPROM.h>

#define EEPROM_SIZE 512
#define LENGTH 32

String name = "LED";

boolean readMqttServer(char * buf){
  int mqtt_server_addr = 0;
  int mqtt_server_length = EEPROMread(mqtt_server_addr, buf);
  Serial.print("Read MqttServer ");
  Serial.println(buf);
  return (mqtt_server_length!=0);
}

boolean readName(char* buf){  
  int name_addr =  LENGTH;
  int name_length = EEPROMread(name_addr, buf);
  Serial.print("Read name ");
  Serial.println(buf);
  return (name_length!=0);
}

//param 0 = mytt server, 1 = Name, 2 = room
void clearEEPROMParam(int param)
{
  int addr = param*LENGTH;
    char data[LENGTH];
    memset(data, '\0', LENGTH);
    EEPROMWrite(addr, data, LENGTH);
}

boolean readRoom(char * buf){
  int room_addr = 2*LENGTH;
  int room_length = EEPROMread(room_addr, buf);
  Serial.print("Read Room ");
  Serial.println(buf);
  return (room_length!=0);
}

void writeMqttServer(char * server){
  int mqtt_server_addr = 0;
  EEPROMWrite(mqtt_server_addr, server, strlen(server));
}

void writeName(char * name){
  int name_addr =  LENGTH;
  EEPROMWrite(name_addr, name, strlen(name));
}

void writeRoom(char * room){
  int room_addr = 2*LENGTH;
  EEPROMWrite(room_addr, room, strlen(room));
}

// reset the EEPROM
void clearEEPROMParam()
{
    char data[EEPROM_SIZE];
    memset(data, '\0', EEPROM_SIZE);
    EEPROMWrite(0, data, EEPROM_SIZE);
}

#define EEPROM_END 0
#define EEPROM_START 1
void EEPROMWrite(int addr, char *data, int size)
{
    EEPROM.begin(EEPROM_SIZE);
    // write the start marker
    EEPROM.write(addr, EEPROM_START);
    addr++;
    for (int i = 0; i < size; i++)
    {
        EEPROM.write(addr, data[i]);
        addr++;
    }
    EEPROM.write(addr, EEPROM_END);
    EEPROM.commit();
    EEPROM.end();
}

// read bytes from addr util '\0'
// return the length of read out.
int EEPROMread(int addr, char *buf)
{
    EEPROM.begin(EEPROM_SIZE);
    int count = -1;
    char c = EEPROM.read(addr);
    addr++;
    if (c != EEPROM_START)
    {
        return 0;
    }
    while (c != EEPROM_END && count < EEPROM_SIZE)
    {
        c = (char)EEPROM.read(addr);
        count++;
        addr++;
        buf[count] = c;
    }
    EEPROM.end();
    return count;
}
