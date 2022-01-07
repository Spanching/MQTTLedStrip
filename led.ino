#include "LEDconfig.h"
#include <FastLED.h>

void fadeOneTowardColor( CRGB& cur, const CRGB& target, uint8_t amount);
void fadeTowardColor( CRGB* L, uint16_t N, const CRGB& bgColor, uint8_t fadeAmount);

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

bool led_state = 1;
bool waves = false;

boolean already_on = false;
boolean already_set_color = false;

CRGBPalette16 currentPalette;
CHSVPalette16 currentPalette_HSV;
TBlendType    currentBlending;

CRGB currentColor = CRGB::Black;
CRGB lastColor = CRGB::White;

CRGB leds[NUM_LEDS];

void colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette);
boolean allLedsSameColor( CRGB* led, uint16_t num, const CRGB& color);
boolean isBlack(const CRGB& color);

void setup_leds(){
  pinMode(LED_PIN, OUTPUT);
  delay( 3000 );
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  currentBlending = LINEARBLEND;
  fill_solid( leds, NUM_LEDS, CRGB::Black);
}

void loop_led(){
  if(led_state > 0 && waves) {
    loop_wave();
  } else if (led_state > 0 && !already_set_color) {
    fadeTowardColor(leds, NUM_LEDS, currentColor, 15);
    if (allLedsSameColor(leds, NUM_LEDS, currentColor)) {
      already_set_color = true;
      if (isBlack(currentColor)) {
        led_state = 0;
      }
    }
    FastLED.show();
  }
  FastLED.delay(2500 / UPDATES_PER_SECOND);
}

void setLEDBrightness(int brightness){
    FastLED.setBrightness(brightness);
}


void loop_wave() {
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }
  
  colorwaves( leds, NUM_LEDS, gCurrentPalette);

  FastLED.show();
}

boolean isBlack(const CRGB& color) {
  return color.r == 0 && color.g == 0 && color.b == 0;
}

boolean allLedsSameColor( CRGB* led, uint16_t num, const CRGB& color) {
  for (int i = 0; i < num; i++) {
    if (led[i] != color) {
      return false;
    }
  }
  return true;
}

void setLEDColor(String rgb_hex){
  waves = false;
  already_set_color = false;
  
  long number = (long) strtol( &rgb_hex[1], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;  
  if (led_state == 0) {
    lastColor = CRGB(r,g,b);
    return;
  }
  lastColor = currentColor;
  currentColor = CRGB(r,g,b);
}

void setLEDWaves(bool state) {
  if (state) {
    waves = false;
    already_set_color = false;
  } else {
    led_state = 1;
    waves = true;
    if (led_state > 0) {
      lastColor = currentColor;
    }
  }
}

void setLEDColorHSV(String hsv_hex) {
  waves = false;
  
  long number = (long) strtol( &hsv_hex[0], NULL, 16);
  byte h = (byte)(number >> 16);
  byte s = (byte)(number >> 8 & 0xFF);
  byte v = (byte)(number & 0xFF);
  Serial.print("HSV: ");
  Serial.println(" " + String(h) + " " + String(s) + " " + String(v));

  CHSV color = CHSV(85, 100, 100);
  CRGB rgb;
  hsv2rgb_rainbow(color, rgb);
  
  already_set_color = false;
  if (led_state == 0) {
    lastColor = rgb;
    return;
  }
  lastColor = currentColor;
  currentColor = rgb;
}

void setLEDPower(boolean on){
  if (on && !already_on && !waves) {
    already_on = true;
    already_set_color = false;
    
    currentColor = lastColor;
    led_state = 1;
  } else if (on && !already_on) {
    led_state = 1;
    currentColor = lastColor;
  }
  if (!on) {
    already_on = false;
    already_set_color = false;
    if (!isBlack(currentColor)) {
      lastColor = currentColor;
      currentColor = CRGB::Black; 
    }
  }
}

// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
void fadeOneTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
}

// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeTowardColor( CRGB* L, uint16_t N, const CRGB& bgColor, uint8_t fadeAmount)
{
  for( uint16_t i = 0; i < N; i++) {
    fadeOneTowardColor( L[i], bgColor, fadeAmount);
  }
}

void setLEDColorTemperature(int ct) {
  if (ct <= 0) {
    FastLED.setTemperature(TEMPERATURE_0);
  }else if (ct == 1) {
    FastLED.setTemperature(TEMPERATURE_1);
  }else if (ct == 2) {
    FastLED.setTemperature(TEMPERATURE_2);
  }else if (ct == 3) {
    FastLED.setTemperature(TEMPERATURE_3);
  }else if (ct == 4) {
    FastLED.setTemperature(TEMPERATURE_4);
  }else if (ct == 5) {
    FastLED.setTemperature(TEMPERATURE_5);
  }else if (ct == 6) {
    FastLED.setTemperature(TEMPERATURE_6);
  }else if (ct == 7) {
    FastLED.setTemperature(TEMPERATURE_7);
  }else if (ct == 8) {
    FastLED.setTemperature(TEMPERATURE_8);
  }else if (ct == 9) {
    FastLED.setTemperature(TEMPERATURE_9);
  }
  FastLED.show();
}
