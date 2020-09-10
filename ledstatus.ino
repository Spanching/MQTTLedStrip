#define LED_STATUS_PIN D2
#define BRIGHTNESS_STATUS 50
#define LED_STATUS_TYPE WS2811

CRGB led[1];

boolean enabled = true;
int ledstatus = 0; //0=connected 1=reconnecting 2=disconnected 

void setup_led_status(){
  pinMode(LED_STATUS_PIN, OUTPUT);     // Initialize the D5 Pin as Output
  //Setup LEDS 
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_STATUS_TYPE, LED_STATUS_PIN>(led, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS_STATUS );

  led[0] = CRGB::Green;
}

void loop_led_status(){
  if(enabled){
    fillLEDwithStatus(ledstatus);
  }
  FastLED.show();
}

void toggleEnabled(){
  enabled = !enabled;
  if(!enabled){
    //disable LED 
    led[0] = CRGB::Black;
  }
  String s = (enabled)?"Status LED enabled":"Status LED disabled";
  Serial.println(s);
}

//Status
//0 green NormalMode
//1 orange WiFiReconnect
//2 red MQTTReconnect
void fillLEDwithStatus(int stat){
  if(stat == 0){
    led[0] = CRGB::Red;
  }else if(stat == 1){
    led[0] = CRGB::Yellow;
  }else if(stat == 2){
    led[0] = CRGB::Green;
  }else{
    led[0] = CRGB::Black;
  }
  FastLED.show();
}

void setStat(int stat){
  ledstatus = stat;
  if(enabled){
    fillLEDwithStatus(ledstatus);
  }
}
