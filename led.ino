#include "LEDconfig.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define TEMPERATURE_0 UncorrectedTemperature
#define TEMPERATURE_1 Candle
#define TEMPERATURE_2 Tungsten40W
#define TEMPERATURE_3 Tungsten100W
#define TEMPERATURE_4 Halogen
#define TEMPERATURE_5 CarbonArc
#define TEMPERATURE_6 HighNoonSun
#define TEMPERATURE_7 DirectSunlight
#define TEMPERATURE_8 OvercastSky
#define TEMPERATURE_9 ClearBlueSky

int led_state = 1;

boolean already_on = false;

CRGBPalette16 currentPalette;
CHSVPalette16 currentPalette_HSV;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];
CRGB correction = CRGB(255, 176, 240);

void setup_leds(){
  pinMode(LED_PIN, OUTPUT);
  delay( 3000 );
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  currentBlending = LINEARBLEND;
  fill_solid( leds, NUM_LEDS, CRGB::White);
}

void loop_led(){
  if (led_state == 0) {
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void setLEDBrightness(int brightness){
    FastLED.setBrightness(brightness);
}

void setLEDColor(String rgb_hex){
    long number = (long) strtol( &rgb_hex[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;  
    
    fill_solid(leds, NUM_LEDS, CRGB(r,g,b));
  FastLED.show();
}

void setLEDColorHSV(String hsv_hex) {
  long number = (long) strtol( &hsv_hex[0], NULL, 16);
  byte h = (byte)(number >> 16);
  byte s = (byte)(number >> 8 & 0xFF);
  byte v = (byte)(number & 0xFF);
  Serial.print("HSV: ");
  Serial.println(" " + String(h) + " " + String(s) + " " + String(v));
  // create a new HSV color
  CHSV color = CHSV(85, 100, 100);
  CRGB rgb;
  hsv2rgb_rainbow(color, rgb);
  // use FastLED to set the color of all LEDs in the strip to the same color
  fill_solid(leds, NUM_LEDS, rgb);
  FastLED.show();
  //hsv = true;
}

void setLEDPower(boolean on){
  if (on && !already_on) {
    already_on = true;
    fill_solid( leds, NUM_LEDS, CRGB::White);
    led_state = 1;
  } 
  if (!on) {
    already_on = false;
    led_state = 0;
    fill_solid( leds, NUM_LEDS, CRGB::Black);
  }
  FastLED.show();
}

void setLEDColorTemperature(int ct) {
  //from 153 to 500
  if (ct == 0) {
    FastLED.setTemperature(TEMPERATURE_1);
  }else
  if (ct == 1) {
    FastLED.setTemperature(TEMPERATURE_9);
  }
}
