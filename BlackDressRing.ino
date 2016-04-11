#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  255
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

#define SOLID_COUNTER 25
#define DELAY 10

CRGB ring[RING_SIZE];
CRGB strap[STRAP_SIZE];
CRGB belt[BELT_SIZE];

// Green yellow blue
// Purple pink blue
// red pink orange purple

void rainbow_march();

void animateStrapsReverse();

void cycle(int colorful, CRGB color);

void cycleReverse(int colorful, CRGB color);

void fadeIn(int rainbow);

void fadeOut(int rainbow);
void runBelt(int rainbow);
void cyleFadeAnimation();
void fillColor(CRGB color);

// Initialize global variables for sequences
uint8_t counter = 0;
uint8_t thishue = 0;
uint8_t deltahue = 10;
uint8_t beltCounter = 0;
int beltLED = 0;

uint8_t solidCounter = 0;

void setup() {
    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<NEOPIXEL, RING>(ring, RING_SIZE);
    FastLED.addLeds<LPD8806, BELT, CLOCK, COLOR_ORDER>(belt, BELT_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    cyleFadeAnimation();
}

void cyleFadeAnimation() {
    fadeIn(1);

    cycle(1, CRGB::Black);
    cycleReverse(0, CRGB::Black);

    fillColor(ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));

    fadeOut(0);
    fadeIn(0);

    solidCounter += SOLID_COUNTER;
    cycleReverse(1, CRGB::Black);
    cycle(0, CRGB::Black);

    fillColor(ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));

    fadeIn(1);
    fadeOut(1);
    solidCounter += SOLID_COUNTER;

    fadeIn(1);
    cycleReverse(0, CRGB::Black);
    cycle(1, CRGB::Black);

    fadeOut(0);
}

void runBelt(int rainbow) {
    if (rainbow == 1) {
        belt[beltLED % BELT_SIZE] = ColorFromPalette(RainbowColors_p, beltCounter, 255, LINEARBLEND);
        beltCounter += 10;
        beltLED++;
    } else {
        fill_solid(belt, BELT_SIZE + 1, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
    }
    FastLED.show();
}

void fillColor(CRGB color) {
    fill_solid(strap, STRAP_SIZE +1, color);
    fill_solid(ring, RING_SIZE +1, color);
}


void cycle(int colorful, CRGB color) {
    int r1 = 15;
    int r2 = 16;
    for (int i = 0; i < TOTAL_PIXELS; i++) {

        if (i < 14) {
            strap[i] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
        } else if (i > 25) {
            strap[i - 12] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
        } else {
            ring[r1] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
            if (r2 == 24) {
                r2 = 0;
            }
            ring[r2] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
            r1--;
            r2++;
        }
        runBelt(1);
        counter += 10;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void cycleReverse(int colorful, CRGB color) {
    int r1 = 4;
    int r2 = 3;
    for (int i = TOTAL_PIXELS; i >= 0; i--) {
        if (i < 14) {
            strap[i] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
        } else if (i > 25) {
            strap[i - 12] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
        } else {
            ring[r1] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
            if (r2 == 0) {
                r2 = 23;
            }
            ring[r2] = colorful == 0 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : color;
            r1++;
            r2--;
        }
        runBelt(1);
        counter += 10;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}


void fadeIn(int rainbow) {
    for (int i = 0; i < BRIGHTNESS; i++) {
        runBelt(rainbow);
        FastLED.setBrightness(i);
        if (rainbow == 1) {
            fill_rainbow(strap, STRAP_SIZE, thishue, deltahue);
            fill_rainbow(ring, RING_SIZE, thishue, deltahue);
        }
        FastLED.show();
    }
}

void fadeOut(int rainbow) {
    for (int i = BRIGHTNESS; i >= 0; i--) {
        runBelt(rainbow);
        FastLED.setBrightness(i);
        if (rainbow == 1) {
            fill_rainbow(strap, STRAP_SIZE, thishue, deltahue);
            fill_rainbow(ring, RING_SIZE, thishue, deltahue);
        }
        FastLED.show();
    }
}