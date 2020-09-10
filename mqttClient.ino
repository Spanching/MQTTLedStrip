#include <PubSubClient.h>
#include <WiFiClient.h>

WiFiClient espClient;

PubSubClient client(espClient);
int value = 0;



char* topic_brightness = "home/bed/led/brightness";
char* topic_power = "home/bedroom/led/power";
char* topic_color = "home/bedroom/led/color";

char* username = MQTT_USER;
char* password = MQTT_PASSWORD;

void setup_mqtt(){
  char * server = "192.168.0.20";
  client.setServer(server, 1883);
  client.setCallback(callback);
  Serial.print("Setup Mqtt with server");
  Serial.println(server);
}

void loop_mqtt(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String str_payload = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str_payload += (char)payload[i]; 
  }
  Serial.println();

  if (strcmp(topic, topic_brightness)==0){
    setBrightness(str_payload.toInt());
  }
  if (strcmp(topic, topic_power)==0){
    if(str_payload == "0") {
      setLEDPower(false);
    } else if (str_payload == "1") {
      setLEDPower(true);
    }
  }
  if (strcmp(topic, topic_color)==0){
    setColor(str_payload);
  }

  char* tmp = (char*)malloc(length+1);
  char* back_topic = (char*)malloc(200);
  memset(back_topic, '\0', 200);
  strcpy(back_topic, topic);
  strcat(back_topic, "/back");
  Serial.println(back_topic);
  str_payload.toCharArray(tmp, length+1);
  client.publish(back_topic, tmp);
  free(tmp);
  free(back_topic);
}

void reconnect() {
  // ledstatus of status led
  setStat(2);
  normal= false;

  // Loop until we're re-/connected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, password)) {
      Serial.println("connected");
      normal = true;
      client.subscribe(topic_brightness);
      client.subscribe(topic_power);
      client.subscribe(topic_color);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
