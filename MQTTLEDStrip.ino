// var for the status led
boolean normal = true;

void setup() {
  Serial.begin(115200);
  setup_led_status();
  setup_wifi(false);
  setup_mqtt();
  setup_leds();
  setup_buttons();
}

void loop() {
  if(normal){
    setStat(0);
  }
  loop_mqtt();
  loop_led();
  loop_led_status();
  loop_buttons();
}
