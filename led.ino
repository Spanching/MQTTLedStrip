#include "LEDconfig.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE


int led_state = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

void setup_leds(){
  pinMode(LED_PIN, OUTPUT);
  delay( 3000 );
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  currentBlending = LINEARBLEND;
  fill_solid( currentPalette, 16, CRGB::Black);
}

void loop_led(){
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  fillLEDsFromPaletteColors(startIndex);
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void fillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void setLEDBrightness(int brightness){
    FastLED.setBrightness(brightness);
}

void setLEDColor(String hex){
    long number = (long) strtol( &hex[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;  
    
    fill_solid(currentPalette, 16, CRGB(r,g,b));
}

void setLEDPower(boolean on){
  if (on) {
    fill_solid( currentPalette, 16, CRGB::White);
  } else {
    fill_solid( currentPalette, 16, CRGB::Black);
  }
}
