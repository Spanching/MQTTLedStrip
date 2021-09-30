void setup() {
  Serial.begin(115200);
  setup_wifi(false);
  setup_mqtt();
  setup_ota();
  setup_leds();
  // setup_buttons();
}

void loop() {
  loop_mqtt();
  loop_led();
  // loop_buttons();
  loop_ota();
}
