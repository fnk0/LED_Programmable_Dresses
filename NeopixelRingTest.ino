#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  255
#define COLOR_ORDER GRB

#define RING_SIZE 24
#define BELT_SIZE 28
#define STRAP_SIZE 16

#define TOTAL_PIXELS 24

#define CLOCK 1

#define RING 10
#define BELT 9
#define STRAP_LEFT 6
#define STRAP_RIGHT 12

#define DELAY 10

CRGB ring[RING_SIZE];
CRGB strap[STRAP_SIZE];
CRGB belt[BELT_SIZE];

void animateRainbow();
void rainbow_march();
void rainbow_beat();
void blendme();
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void SetupRandomPalette();

uint8_t colorIndex = 0;

// Initialize global variables for sequences
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.

uint8_t counter = 0;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
uint8_t maxChanges = 24;
TBlendType currentBlending;

#define PALETTE_SIZE 7

CRGBPalette16 palettes[] = {
        RainbowColors_p, CloudColors_p, OceanColors_p, RainbowStripeColors_p, ForestColors_p, HeatColors_p, RainbowColors_p
};

void setup() {
    delay(3000); // power-up safety delay
    Serial.begin(57600);

    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(strap, STRAP_SIZE);
    FastLED.addLeds<NEOPIXEL, RING>(ring, RING_SIZE);
    FastLED.addLeds<LPD8806, BELT, CLOCK, COLOR_ORDER>(belt, BELT_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    rainbow_march();
    rainbow_beat();
    blendme();

    show_at_max_brightness_for_power();
}

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
    thishue++;                                                  // Increment the starting hue.
    fill_rainbow(ring, RING_SIZE, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
    fill_rainbow(strap, STRAP_SIZE, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
    fill_rainbow(belt, BELT_SIZE, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
}

void rainbow_beat() {
    uint8_t beatA = beatsin8(5, 0, 255);
    uint8_t beatB = beatsin8(15, 4, 20);
    fill_rainbow(ring, RING_SIZE, beatA, beatB);
    fill_rainbow(strap, STRAP_SIZE, beatA, beatB);
    fill_rainbow(belt, BELT_SIZE, beatA, beatB);
} // rainbow_beat()

void animateRainbow() {
    for (uint8_t i = 0; i < TOTAL_PIXELS; i++) {

        ring[i] = ColorFromPalette(RainbowColors_p, colorIndex, 255, LINEARBLEND);
        if (i < STRAP_SIZE) {
            strap[i] = ColorFromPalette(RainbowColors_p, colorIndex, 255, LINEARBLEND);
        }

        if (i < BELT_SIZE) {
            belt[i] = ColorFromPalette(RainbowColors_p, colorIndex, 255, LINEARBLEND);
        }
        colorIndex += 3;
        FastLED.show();
        FastLED.delay(DELAY);
    }
}

void blendme() {
    uint8_t starthue = beatsin8(20, 0, 255);
    uint8_t endhue = beatsin8(35, 0, 255);
    if (starthue < endhue) {
        fill_gradient(ring, RING_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), FORWARD_HUES);
        fill_gradient(strap, STRAP_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), FORWARD_HUES);
        fill_gradient(belt, BELT_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), FORWARD_HUES);
    } else {
        fill_gradient(ring, RING_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), BACKWARD_HUES);
        fill_gradient(strap, STRAP_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), BACKWARD_HUES);
        fill_gradient(belt, BELT_SIZE, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), BACKWARD_HUES);
    }
} // blendme()

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t beatB = beatsin8(15, 10, 20);                       // Delta hue between LED's
    for (int i = 0; i < TOTAL_PIXELS; i++) {
        ring[i] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
        if (i < STRAP_SIZE) {
            strap[i] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
        }

        if (i < BELT_SIZE) {
            belt[i] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
        }
        colorIndex += beatB;
        FastLED.show();
        FastLED.delay(DELAY);

    }
} //FillLEDsFromPaletteColors()


void SetupRandomPalette() {
    targetPalette = palettes[counter % PALETTE_SIZE];
    counter++;
}