#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  50
#define TOTAL_PIXELS 32
#define DATA 3
#define DELAY 10

CRGB feather[TOTAL_PIXELS];

uint8_t hue;
uint8_t delta;
int counter = 0;

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA>(feather, TOTAL_PIXELS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

//    fill_rainbow(feather, TOTAL_PIXELS, hue, delta);
//    hue++;
//    delta++;

    for(int i= 0; i < 4; i++) {
        for(int j = 0; j< 8; j++) {
            feather[j + 8] = ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND);
        }
        counter+= 50;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}