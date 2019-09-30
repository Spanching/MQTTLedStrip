#include "LEDconfig.h"
#include <FastLED.h>

// ugly in here, still work to do

int led_state = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

void setup_leds(){
  pinMode(LED_PIN, OUTPUT);     // Initialize the D6 Pin as Output
  //Setup LEDS 
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
    
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  fill_solid( currentPalette, 16, CRGB::Black);
}

void loop_led(){
    //LOOP for LEDs
  //ChangePalettePeriodically();
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void setBrightness(int brightness){
    FastLED.setBrightness(brightness);
}

void setStripColor(int color, int br){
  if(br<=255&&br>=0){
    setBrightness(br);
  }
  switch(color){
    case 0:
    // white
      fill_solid( currentPalette, 16, CRGB::White);
    break;
    case 1:
    // green
      fill_solid( currentPalette, 16, CRGB::Green);
    break;
    case 2:
    //red
      fill_solid( currentPalette, 16, CRGB::Red);
    break;
    case 3:
    //blue
      fill_solid( currentPalette, 16, CRGB::Blue);
    break;
    case 4:
    //yellow
      fill_solid( currentPalette, 16, CRGB::Yellow);
    break;  
    case 5:
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    break;
    default:
      fill_solid( currentPalette, 16, CRGB::Black);
    break;
  }
  
}

void setStrip(char c, String b) {
  String message = "";
  int br = b.toInt();
  Serial.print("Brightness setting: " + br);
  Serial.println(" mode " + c);
  if(br<=255&&br>=0){
    setBrightness(br);
  }
  if(c=='0'){
      if(led_state==0){
        Serial.println("LED still OFF.");
        message += "LED:  0 -> 0";
      }else if(led_state==1){
        Serial.println("LED set to OFF now.");
        message += "LED:  1 -> 0";
        led_state = 0;
        setPalette(0);
      }else{
        Serial.println("LED set to OFF now.");
        message += "LED: 2 -> 0";
        led_state = 0;
        setPalette(0);
      }
  }else if(c=='1'){
       if(led_state==0){
        Serial.println("LED set to ON now.");
        message += "LED:  0 -> 1";
        led_state = 1;
        setPalette(1);
      }else if(led_state==1){
        Serial.println("LED still ON.");
        message += "LED:  1 -> 1";
      }else{
        Serial.println("LED set to ON now.");
        message += "LED: 2 -> 1";
        led_state = 1;
        setPalette(1);
      }
  }else if(c=='2'){
      if(led_state==0){
        Serial.println("LED set to RAINBOW now.");
        message += "LED:  0 -> 2";
        led_state = 2;
        setPalette(2);
      }else if(led_state==1){
        Serial.println("LED set to RAINBOW now.");
        message += "LED:  1 -> 2";
        led_state = 2;
        setPalette(2);
      }else{
        Serial.println("LED still RAINBOW.");
        message += "LED: 2 -> 2";
      }
  }
}

//Code 0 : Black
//Code 1 : White
//Code 2 : Rainbow
void setPalette(int code){
    switch(code){
      case 0:
        fill_solid( currentPalette, 16, CRGB::Black);
      break;
      case 1:
        fill_solid( currentPalette, 16, CRGB::White);
      break;
      case 2:
        currentPalette = RainbowColors_p;         
        currentBlending = LINEARBLEND;
      break;
      default:
        fill_solid( currentPalette, 16, CRGB::White);
      break;
    }
}
