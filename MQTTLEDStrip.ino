// var for the status led
boolean normal = true;

void setup() {
  Serial.begin(115200);
  //setup_led_status();
  //setStat(4);
  setup_wifi(false);
  //setStat(4);
  //setup_mqtt();
  //setStat(4);
  setup_leds();
  //setStat(4);
  //setup_buttons();
  setup_fauxmo();
}

void loop() {
  if(normal){
    //setStat(0);
  }
  //loop_mqtt();
  loop_led();
  //loop_led_status();
  //loop_buttons();
  loop_fauxmo();
}
