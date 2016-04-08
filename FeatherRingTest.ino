#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  255
#define COLOR_ORDER GRB

#define DATA 1
#define SIZE 24

CRGB ring[SIZE];

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA>(ring, SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

cycleColor(CRGB::Red);
cycleColor(CRGB::Blue);
cycleColor(CRGB::Yellow);
cycleColor(CRGB::Violet);
cycleColor(CRGB::Green);
cycleColor(CRGB::Pink);
cycleColor(CRGB::Aqua);

}

void cycleColor(CRGB color) {
    for(int i = 0; i < SIZE; i++) {
        ring[i] = color;
    }

    for(int i = 0; i < SIZE; i++) {
        ring[i] = CRGB::Black;
    }
}