#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  125
#define COLOR_ORDER GRB

#define RING_SIZE 24
#define BELT_SIZE 22
#define STRAP_SIZE 16

#define TOTAL_PIXELS 28

#define CLOCK 1

#define RING 10
#define BELT 9
#define STRAP_LEFT 6
#define STRAP_RIGHT 12

#define DELAY 10

CRGB ring[RING_SIZE];
CRGB strap[STRAP_SIZE];
CRGB belt[BELT_SIZE];

// Green yellow blue
// Purple pink blue
// red pink orange purple

void rainbow_march();

void animateStrapsReverse();

void cycle(int colorful);

void cycleReverse(int colorful);

void fadeIn();

void fadeOut();

void runBelt();

// Initialize global variables for sequences
uint8_t counter = 0;
uint8_t thishue = 0;
uint8_t deltahue = 10;
uint8_t beltCounter = 0;
int beltLED = 0;

void setup() {
    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<NEOPIXEL, RING>(ring, RING_SIZE);
    FastLED.addLeds<LPD8806, BELT, CLOCK, COLOR_ORDER>(belt, BELT_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    fadeIn();
    fadeOut();
    fadeIn();

    cycle(1);
    cycleReverse(0);
    fadeIn();
    fadeOut();
    fadeIn();

    cycleReverse(1);
    cycle(0);
}

void runBelt() {
    belt[beltLED % BELT_SIZE] = ColorFromPalette(RainbowColors_p, beltCounter, 255, LINEARBLEND);
    beltCounter += 10;
    beltLED++;
    FastLED.show();
}

void cycle(int colorful) {
    int r1 = 15;
    int r2 = 16;
    for (int i = 0; i < TOTAL_PIXELS; i++) {

        if (i < 14) {
            strap[i] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        } else if (i > 25) {
            strap[i - 12] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        } else {
            ring[r1] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
            if (r2 == 24) {
                r2 = 0;
            }
            ring[r2] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
            r1--;
            r2++;
        }
        runBelt();
        counter += 10;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void cycleReverse(int colorful) {
    int r1 = 4;
    int r2 = 3;
    for (int i = TOTAL_PIXELS; i >= 0; i--) {
        if (i < 14) {
            strap[i] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        } else if (i > 25) {
            strap[i - 12] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        } else {
            ring[r1] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
            if (r2 == 0) {
                r2 = 23;
            }
            ring[r2] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
            r1++;
            r2--;
        }
        runBelt();
        counter += 10;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void fadeIn() {
    for (int i = 0; i <= BRIGHTNESS; i++) {
        runBelt();
        FastLED.setBrightness(i);
        fill_rainbow(strap, STRAP_SIZE, thishue, deltahue);
        fill_rainbow(ring, RING_SIZE, thishue, deltahue);
        FastLED.show();
    }
}

void fadeOut() {
    for (int i = 255; i >= BRIGHTNESS; i--) {
        runBelt();
        FastLED.setBrightness(i);
        fill_rainbow(strap, STRAP_SIZE, thishue, deltahue);
        fill_rainbow(ring, RING_SIZE, thishue, deltahue);
        FastLED.show();
    }
}