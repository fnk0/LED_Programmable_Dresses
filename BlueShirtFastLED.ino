#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM303.h>
#include "FastLED/FastLED.h"

#define BRIGHTNESS 225
#define BOARD 8
#define LEDS_SIZE 25
#define LEDS 9
#define TOTAL_PIXELS 26

CRGB leds[TOTAL_PIXELS];


#define FAVCOLORS 6

CRGB myFavoriteColors[FAVCOLORS] ={
        CRGB::Green,
        CRGB::Red,
        CRGB::Aqua,
        CRGB::Violet,
        CRGB::DeepPink,
        CRGB::White
};


// mess with this number to adjust TWINklitude :)
// lower number = more sensitive
#define MOVE_THRESHOLD 20

void flashRandom(int wait, uint8_t howmany);

void setup() {

    FastLED.addLeds<NEOPIXEL, LEDS>(leds, 0, LEDS_SIZE);
    FastLED.addLeds<NEOPIXEL, BOARD>(leds, LEDS_SIZE, 1);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    // Try to initialise and warn if we couldn't detect the chip
    if (!lsm.begin()) {
        Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
        while (1);
    }
}

void loop() {

    // Take a reading of accellerometer data
    lsm.read();

    // Get the magnitude (length) of the 3 axis vector
    // http://en.wikipedia.org/wiki/Euclidean_vector#Length
    double storedVector = lsm.accelData.x * lsm.accelData.x;
    storedVector += lsm.accelData.y * lsm.accelData.y;
    storedVector += lsm.accelData.z * lsm.accelData.z;
    storedVector = sqrt(storedVector);

    // get new data!
    lsm.read();
    double newVector = lsm.accelData.x * lsm.accelData.x;
    newVector += lsm.accelData.y * lsm.accelData.y;
    newVector += lsm.accelData.z * lsm.accelData.z;
    newVector = sqrt(newVector);

    // are we moving
    if (abs(newVector - storedVector) > MOVE_THRESHOLD) {
        Serial.println("Twinkle!");
        flashRandom(10, 6);  // first number is 'wait' delay, shorter num == shorter twinkle
        flashRandom(10, 8);  // second number is how many neopixels to simultaneously light up
        flashRandom(10, 10);
    }

}


void flashRandom(int wait, uint8_t howmany) {

    for (uint16_t i = 0; i < howmany; i++) {
        // pick a random favorite color!
        int c = random(FAVCOLORS);
        int red = myFavoriteColors[c][0];
        int green = myFavoriteColors[c][1];
        int blue = myFavoriteColors[c][2];

        // get a random pixel from the list
        int j = random(strip.numPixels());
        //Serial.print("Lighting up "); Serial.println(j);

        // now we will 'fade' it in 5 steps
        for (int x = 0; x < 5; x++) {
            int r = red * (x + 1);
            r /= 5;
            int g = green * (x + 1);
            g /= 5;
            int b = blue * (x + 1);
            b /= 5;

            leds[j] =

            strip.setPixelColor(j, strip.Color(r, g, b));
            strip.show();
            delay(wait);
        }
        // & fade out in 5 steps
        for (int x = 5; x >= 0; x--) {
            int r = red * x;
            r /= 5;
            int g = green * x;
            g /= 5;
            int b = blue * x;
            b /= 5;

            strip.setPixelColor(j, strip.Color(r, g, b));
            strip.show();
            delay(wait);
        }
    }
    // LEDs will be off when done (they are faded to 0)
}