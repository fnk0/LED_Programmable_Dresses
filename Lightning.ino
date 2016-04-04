#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  70
#define COLOR_ORDER GRB

#define RING_SIZE 24
#define BELT_SIZE 23
#define STRAP_SIZE 16

#define TOTAL_PIXELS 24

#define CLOCK 1

#define RING 10
#define BELT 6
#define STRAP_LEFT 12
#define STRAP_RIGHT 9

#define DELAY 100

CRGB leds[TOTAL_PIXELS];

uint8_t max_bright = 128;
uint8_t frequency = 10;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;

uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;                                               // Length of a flash


void setup() {
    delay(3000); // power-up safety delay
    Serial.begin(57600);

    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(leds, TOTAL_PIXELS);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(leds, TOTAL_PIXELS);
    FastLED.addLeds<NEOPIXEL, RING>(leds, TOTAL_PIXELS);
    FastLED.addLeds<LPD8806, BELT, CLOCK, COLOR_ORDER>(leds, TOTAL_PIXELS);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    ledstart = random8(TOTAL_PIXELS);           // Determine starting location of flash
    ledlen = random8(TOTAL_PIXELS - ledstart);    // Determine length of flash (not to go beyond TOTAL_PIXELS-1)
    for (int flashCounter = 0; flashCounter < random8(3, flashes); flashCounter++) {
        if (flashCounter == 0) dimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
        else dimmer = random8(1, 3);           // return strokes are brighter than the leader
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
        FastLED.show();                       // Show a section of LED's
        delay(random8(4, 10));                 // each flash only lasts 4-10 milliseconds
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 0));   // Clear the section of LED's
        FastLED.show();
        if (flashCounter == 0) delay(150);   // longer delay until next flash after the leader
        delay(50 + random8(100));               // shorter delay between strokes
    } // for()
    delay(random8(frequency) * 100);          // delay between strikes
}