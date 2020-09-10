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

int led_state = 0;

boolean already_on = false;

CRGBPalette16 currentPalette;
CHSVPalette16 currentPalette_HSV;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

void setup_leds(){
  pinMode(LED_PIN, OUTPUT);
  delay( 3000 );
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  currentBlending = LINEARBLEND;
  fill_solid( leds, NUM_LEDS, CRGB::Black);
}

void loop_led(){    
  FastLED.show();
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
  //hsv = true;
}

void setLEDPower(boolean on){
  if (on && !already_on) {
    already_on = true;
    fill_solid( leds, NUM_LEDS, CRGB::White);
  } else {
    already_on = false;
    fill_solid( leds, NUM_LEDS, CRGB::Black);
  }
}

void setLEDColorTemperature(int kelvin) {
  if (kelvin < 2250) {
    FastLED.setTemperature(TEMPERATURE_1);
  } else if (kelvin < 2725) {
    FastLED.setTemperature(TEMPERATURE_2);
  } else if (kelvin < 3025) {
    FastLED.setTemperature(TEMPERATURE_3);
  } else if (kelvin < 4200) {
    FastLED.setTemperature(TEMPERATURE_4);
  } else if (kelvin < 5300) {
    FastLED.setTemperature(TEMPERATURE_5);
  } else if (kelvin < 5700) {
    FastLED.setTemperature(TEMPERATURE_6);
  } else if (kelvin < 6500) {
    FastLED.setTemperature(TEMPERATURE_7);
  } else if (kelvin < 13500) {
    FastLED.setTemperature(TEMPERATURE_8);
  } else {
    FastLED.setTemperature(TEMPERATURE_9);
  }
}
