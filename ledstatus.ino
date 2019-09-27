#define LED_STATUS_PIN D5
#define BRIGHTNESS_STATUS 50
#define LED_STATUS_TYPE WS2811
CRGB led[1];

boolean enabled = false;
int status = 0; //0=connected 1=reconnecting 2=disconnected 

void setup_led_status(){
  pinMode(LED_STATUS_PIN, OUTPUT);     // Initialize the D5 Pin as Output
  //Setup LEDS 
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_STATUS_TYPE, LED_PIN>(led, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS_STATUS );

  led[0] = CRGB::Black;
}

void loop_led_status(){
  if(enabled){
    fillLEDwithStatus(status);
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
//3 blue AP Mode
//4 lightBlue Setup
void fillLEDwithStatus(int stat){
  if(stat == 0){
    led[0] = CRGB::Green;
  }else if(stat == 1){
    led[0] = CRGB::Orange;
  }else if(stat == 2){
    led[0] = CRGB::Red;
  }else if(stat == 2){
    led[0] = CRGB::Blue;
  }else if(stat == 4){
    led[0] = CRGB::LightBlue;
  }
}

void setStat(int stat){
  status = stat;
}
