#include <Arduino.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS  64
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 100

#define SMALL_STRAP_SIZE 18
#define BIG_STRAP_SIZE 20
#define MIDDLE_STRAP_SIZE 19

#define TOTAL_PIXELS 99

#define CLOCK 0

#define RING 10
#define BELT 6
#define STRAP_LEFT 12
#define STRAP_RIGHT 9

#define DELAY 10

// Define the array of leds
CRGB leds[BIG_STRAP_SIZE];

void setup() {
    delay(3000); // power-up safety delay

    Serial.begin(57600);

    FastLED.addLeds<LPD8806, BELT, CLOCK, COLOR_ORDER>(leds, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(leds, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(leds, BIG_STRAP_SIZE);
//    FastLED.addLeds<NEOPIXEL, RING>(leds, 24);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}
/* pop_fade_demo
By: Andrew Tuline
Date: Sep, 2014
A demo showing the flexibility of individual LED pop fades across the HSV range. A little code and lots of variables can go a long way.
1) Fade
2) Colour bars.
3) Colour randomness, or array of colours. Set random via RGB for a wide range of colours.
4) Delay
You can set an LED with HSV values, but you can't query those values and increment/decrement them unless you maintain
a separate array. In addtion, there are no HSV methods/functions available yet in FastLED. As a result, I'm not doing any HSV modifications,
such as changing the saturation (which doesn't look at that cool anyway).
*/

#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

// Most of these variables can be changed.
int ranamount = 50;                                           // The higher the number, lowers the chance for a pixel to light up.
uint8_t thisdelay = 50;                                       // Standard delay value.
bool boolcolours = 1;                                         // True = random, False = array of colours (up to 10)
uint8_t numcolours = 2;                                       // Number of colours in the array to display.
unsigned long colours[10] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff};  // Just assign the first 3 of these selectable colours.
uint8_t maxbar = 1;                                           // Maximum # of pixels to turn on in a row
uint8_t fadeval = 224;                                        // Fade rate

void loop() {
//    ChangeMe();                                                 // Muck those variable around.
//    EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
//        pop_fade();                                                 // Simple call to the routine.
//    }
//    show_at_max_brightness_for_power();

    for(int i = 0; i < BIG_STRAP_SIZE; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
        FastLED.delay(100);
    }

    for(int i = 0; i < BIG_STRAP_SIZE; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        FastLED.delay(100);
    }

    for(int i = 0; i < BIG_STRAP_SIZE; i++) {
        leds[i] = CRGB::Green;
        FastLED.show();
        FastLED.delay(100);
    }

} // loop()



void pop_fade() {
    unsigned long thiscolour;
    if (ranamount >NUM_LEDS) ranamount = NUM_LEDS;               // Make sure we're at least utilizing ALL the LED's.
    int idex = random16(0, ranamount);

    if (idex < NUM_LEDS) {                                      // Only the lowest probability twinkles will do.
        boolcolours ? thiscolour = random(0, 0xffffff) : thiscolour =  colours[random16(0, numcolours)];
        int barlen = random16(1,maxbar);
        for (int i = 0; i <barlen; i++)
            if (idex+i < NUM_LEDS) leds[idex+i] = thiscolour;       // Make sure we don't overshoot the array.
    }
    for (int i = 0; i <NUM_LEDS; i++) leds[i].nscale8(fadeval); // Go through the array and reduce each RGB value by a percentage.
} // pop_fade()



void ChangeMe() {
    uint8_t secondHand = (millis() / 1000) % 30;                // Increase this if you want a longer demo.
    static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

    // You can change variables, but remember to set them back in the next demo, or they will stay as is.
    if (lastSecond != secondHand) {
        lastSecond = secondHand;
        switch(secondHand) {
            case  0: thisdelay = 50; colours[0] = 0xffffff; numcolours=1; boolcolours=0; maxbar = 1; break;          // Just white twinkles
            case  5: thisdelay = 20; colours[1] = 0xff0000; numcolours=2; boolcolours=0; maxbar = 4; break;          // Add red and make bars
            case 15: thisdelay = 50; boolcolours=1; maxbar=1; break;                                                 // A pile of colours, 1 pixel in length
            case 20: thisdelay = 50; fadeval = 128; break;                                                           // Slow down the fade
            case 25: thisdelay = 50; colours[2]= 0x0000ff; boolcolours=0; numcolours=3; fadeval = 192; maxbar = 6; break;
            case 30: break;
        }
    } // if lastSecond
} // ChangeMe()