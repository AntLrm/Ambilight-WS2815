#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2 // or D2

CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < 60; i ++){
    leds[i] = CRGB(100,0,0);
  }
  FastLED.show();
}
 
void loop() { 
  if (Serial.available() > 0){
    for (int i = 0; i < NUM_LEDS; i ++){
      if (Serial.available() > 0){
        char b = Serial.read();
        char r = Serial.read();
        char g = Serial.read();
        leds[i] = CRGB(r, g, b);
        FastLED.show();
      }
    }
  }
}
