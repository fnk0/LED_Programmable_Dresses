#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  200
#define COLOR_ORDER GRB

#define SMALL_STRAP_SIZE 16
#define BIG_STRAP_SIZE 20
#define MIDDLE_STRAP_SIZE 18

#define MIDDLE_SHOULDER_SIZE 38
#define TOTAL_PIXELS 20

#define CLOCK 0

#define SMALL_LEFT 10
#define BIG_LEFT 6
#define SMALL_RIGHT 12
#define BIG_RIGHT 9
#define MIDDLE 1

#define DELAY 10
#define DELAY_PULSE 1

#define SOLID_COUNTER 25
#define GLOBAL_COUNTER 15

CRGB smallStrap[SMALL_STRAP_SIZE];
CRGB bigStrap[BIG_STRAP_SIZE];
CRGB middleStrap[MIDDLE_STRAP_SIZE];

void cycle(int rainbow);
void cycleReverse(int rainbow);

void fadeIn(int rainbow, CRGB color);
void fadeOut(int rainbow, CRGB color);

void cycleFromCenter(int rainbow, CRGB color);
void cycleReverseToCenter(int rainbow, CRGB color);

void fillRainbow();
void fillSolid(CRGB color);

uint8_t thishue = 0;
uint8_t deltahue = 10;

uint8_t centerCounter = 0;
uint8_t reverseCenterCounter = 0;

uint8_t counter = 0;
uint8_t solidCounter = 0;

int loopCounter = 0;

CRGB rainbowColors[MIDDLE_SHOULDER_SIZE];

void setup() {
    FastLED.addLeds<LPD8806, SMALL_LEFT, CLOCK, COLOR_ORDER>(smallStrap, SMALL_STRAP_SIZE);
    FastLED.addLeds<LPD8806, SMALL_RIGHT, CLOCK, COLOR_ORDER>(smallStrap, SMALL_STRAP_SIZE);
    FastLED.addLeds<LPD8806, BIG_LEFT, CLOCK, COLOR_ORDER>(bigStrap, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, BIG_RIGHT, CLOCK, COLOR_ORDER>(bigStrap, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, MIDDLE, CLOCK, COLOR_ORDER>(middleStrap, MIDDLE_STRAP_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);


    uint8_t count = 0;

    for(int i = 0; i < MIDDLE_SHOULDER_SIZE; i++) {
        rainbowColors[i] = ColorFromPalette(RainbowColors_p, count, 255, LINEARBLEND);
        count += GLOBAL_COUNTER / 2;
    }
}

void loop() {

//    if (loopCounter % 8  == 0) {
//        cycle(1);
//        cycleReverse(1);
//    }
//
//    fadeOut(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
//    solidCounter += SOLID_COUNTER;
//    fadeIn(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
//    solidCounter += SOLID_COUNTER;
//
//    loopCounter++;

    cycleFromCenter(1, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
//    solidCounter += SOLID_COUNTER;
    cycleFromCenter(0, CRGB::Black);

    cycleReverseToCenter(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
    solidCounter += SOLID_COUNTER;
    cycleReverseToCenter(0, CRGB::Black);

}

void cycle(int rainbow) {

    for (int i = MIDDLE_STRAP_SIZE; i >= 0; i--) {
        middleStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        FastLED.delay(DELAY);
    }

    for (int i = BIG_STRAP_SIZE; i >= 0; i--) {
        if (i < 18) {
            smallStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        }
        bigStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void cycleReverse(int rainbow) {

    for (int i = 0; i < BIG_STRAP_SIZE; i++) {
        if (i > 2) {
            smallStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        }
        bigStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        FastLED.delay(DELAY);
    }

    for (int i = 0; i < MIDDLE_STRAP_SIZE; i++) {
        middleStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void cycleFromCenter(int rainbow, CRGB color) {

    for(int i = 0; i < TOTAL_PIXELS; i++) {

        if (rainbow == 1) {
            if (i <= SMALL_STRAP_SIZE) {
                smallStrap[SMALL_STRAP_SIZE - i] = rainbowColors[i + MIDDLE_STRAP_SIZE + 2];
            }

            if (i < MIDDLE_STRAP_SIZE) {
                middleStrap[i] = rainbowColors[MIDDLE_STRAP_SIZE - i];
            }

            bigStrap[BIG_STRAP_SIZE - i] = rainbowColors[BIG_STRAP_SIZE - i];
        } else {
            if (i <= SMALL_STRAP_SIZE) {
                smallStrap[SMALL_STRAP_SIZE - i] = color;
            }

            if (i < MIDDLE_STRAP_SIZE) {
                middleStrap[i] = color;
            }

            bigStrap[BIG_STRAP_SIZE - i] = color;
        }

        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void cycleReverseToCenter(int rainbow, CRGB color) {

    for(int i = 0; i < TOTAL_PIXELS; i++) {
        if (i < SMALL_STRAP_SIZE) {
            smallStrap[i] = color;
        }

        if (i <= MIDDLE_STRAP_SIZE) {
            middleStrap[MIDDLE_STRAP_SIZE - i] = color;
        }

        bigStrap[i] = color;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void fadeIn(int rainbow, CRGB color) {
    for (int i = 0; i <= BRIGHTNESS; i++) {
        FastLED.setBrightness(i);
        if (rainbow == 1) {
            fillRainbow();
        } else {
            fillSolid(color);
        }
        FastLED.show();
    }
}

void fadeOut(int rainbow, CRGB color) {
    for (int i = BRIGHTNESS; i >= 0; i--) {
        FastLED.setBrightness(i);
        if (rainbow == 1) {
            fillRainbow();
        } else {
            fillSolid(color);
        }
        FastLED.show();
    }
}

void fillRainbow() {
    fill_rainbow(smallStrap, SMALL_STRAP_SIZE, thishue, deltahue);
    fill_rainbow(bigStrap, BIG_STRAP_SIZE, thishue, deltahue);
    fill_rainbow(middleStrap, MIDDLE_STRAP_SIZE, thishue, deltahue);
}

void fillSolid(CRGB color) {
    fill_solid(smallStrap, SMALL_STRAP_SIZE, color);
    fill_solid(bigStrap, BIG_STRAP_SIZE, color);
    fill_solid(middleStrap, MIDDLE_STRAP_SIZE, color);
}
