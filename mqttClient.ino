#include <PubSubClient.h>

PubSubClient client(espClient);
int value = 0;

void setup_mqtt(){
  char * server = (char *) malloc(32);
  readMqttServer(server);
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
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  char led_state = (char)payload[0];
  Serial.println("state " + led_state);
  String brightness = "";
  for(int i = 2; i < length; i++){
    brightness = brightness + (char)payload[i];
  }
  Serial.println("brightness " + brightness);
   
  setStrip(led_state, brightness);
}

void reconnect() {
  setStat(2);
  normal= false;
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      normal = true;
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("home/livingroom/mid-strip");
      client.subscribe("outTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
