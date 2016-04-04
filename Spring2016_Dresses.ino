#include <Arduino.h>
#include "LPD8806.h"
#include "SPI.H"
#include <Adafruit_NeoPixel.h>


#define SMALL_STRAP_SIZE 18
#define BIG_STRAP_SIZE 20
#define MIDDLE_STRAP_SIZE 19

#define TOTAL_PIXELS 99

#define CLOCK 0

#define SMALL_LEFT 10
#define BIG_LEFT 6
#define SMALL_RIGHT 12
#define BIG_RIGHT 9
#define MIDDLE 1

#define DELAY 10

LPD8806 leftSmallStrip = LPD8806(SMALL_STRAP_SIZE, SMALL_LEFT, CLOCK);
LPD8806 leftBigStrip = LPD8806(BIG_STRAP_SIZE, BIG_LEFT, CLOCK);
LPD8806 rightSmallStrip = LPD8806(SMALL_STRAP_SIZE, SMALL_RIGHT, CLOCK);
LPD8806 rightBigStrip = LPD8806(BIG_STRAP_SIZE, BIG_RIGHT, CLOCK);
LPD8806 middleStrip = LPD8806(MIDDLE_STRAP_SIZE, MIDDLE, CLOCK);

int gamma[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
        5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
        10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
        17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
        25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
        37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
        51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
        69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
        90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
        115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
        144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
        177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
        215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};


// Function prototypes
void startStrips();

void showStrips();

void animateMiddle(uint8_t wait);

void animateStraps(uint8_t wait);

void animateMiddleReverse(uint8_t wait);

void animateStrapsReverse(uint8_t wait);

void colorWipe(uint32_t c, uint8_t wait, LPD8806 strip);

void colorWipeStraps(uint32_t c, uint8_t wait);

void stepColorReverse(uint8_t wait, uint16_t color, LPD8806 strip);

void stepColorMultipleStripsReverse(uint8_t wait, uint16_t color, LPD8806 strips[]);

uint32_t Wheel(uint16_t WheelPos, LPD8806 strip);

void pulse(uint8_t wait);

LPD8806 bigs[] = {leftBigStrip, leftSmallStrip, rightBigStrip, rightSmallStrip};
LPD8806 allStrips[] = {leftBigStrip, leftSmallStrip, rightBigStrip, rightSmallStrip, middleStrip};
uint32_t colors[] = {
        middleStrip.Color(0, 0, 0),
        middleStrip.Color(0, 255, 0),
        middleStrip.Color(0, 0, 255),
        middleStrip.Color(0, 255, 0),
        middleStrip.Color(0, 0, 0)
};

void setup() {
    delay(1);
    startStrips();
    showStrips();
}

void loop() {
    animateMiddle(DELAY);
    animateStraps(DELAY);

//    stepColorMultipleStrips(DELAY, leftBigStrip.Color(0, 0, 0), bigs);
//    stepColor(DELAY, middleStrip.Color(0, 0, 0), middleStrip);

    animateStrapsReverse(DELAY);
    animateMiddleReverse(DELAY);

    colorWipe(middleStrip.Color(0, 0, 0), DELAY, middleStrip);
    colorWipeStraps(middleStrip.Color(0, 0, 0), DELAY);
//    stepColor(DELAY, middleStrip.Color(0, 0, 0), middleStrip);
//    stepColorMultipleStrips(DELAY, leftBigStrip.Color(0, 0, 0), bigs);

//    stepColorReverse(DELAY, middleStrip.Color(0, 0, 0), middleStrip);
    stepColorMultipleStripsReverse(DELAY, leftBigStrip.Color(0, 0, 0), allStrips);
}


void startStrips() {
    leftSmallStrip.begin();
    leftBigStrip.begin();
    rightSmallStrip.begin();
    rightBigStrip.begin();
    middleStrip.begin();
}

void showStrips() {
    leftSmallStrip.show();
    leftBigStrip.show();
    rightSmallStrip.show();
    rightBigStrip.show();
    middleStrip.show();
}

void stepColor(uint8_t wait, uint16_t color, LPD8806 strip) {
    for (int i = strip.numPixels(); i >= 0; i--) {
        strip.setPixelColor(i, color);
        strip.show();
        delay(wait);
    }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait, LPD8806 strip) {
    uint16_t i;

    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorWipeStraps(uint32_t c, uint8_t wait) {
    for(uint16_t i = 0; i < leftBigStrip.numPixels(); i++) {
        leftBigStrip.setPixelColor(i, c);
        rightBigStrip.setPixelColor(i, c);
    }

    for(int i = 0; i < leftSmallStrip.numPixels(); i++) {
        leftSmallStrip.setPixelColor(i, c);
        rightSmallStrip.setPixelColor(i, c);
    }

    delay(wait);
    showStrips();
}


void animateMiddle(uint8_t wait) {
    uint16_t i, j, p;
    p = MIDDLE_STRAP_SIZE;
    for (i = 0; i <= MIDDLE_STRAP_SIZE; i++) {
        uint32_t color = Wheel(((i * 384 / middleStrip.numPixels()) + j) % 384, middleStrip);
        middleStrip.setPixelColor(p, color);
        colors[i] = color;
        p--;
        middleStrip.show();
        delay(wait);
    }
}

void animateStraps(uint8_t wait) {
    uint16_t i, p, x;
    p = BIG_STRAP_SIZE;
    x = MIDDLE_STRAP_SIZE;
    for (i = 0; i <= BIG_STRAP_SIZE; i++) {
        uint32_t color = Wheel(((x * 384 / leftBigStrip.numPixels())) % 384, leftBigStrip);
        colors[x] = color;
        leftSmallStrip.setPixelColor(p, color);
        leftBigStrip.setPixelColor(p, color);
        rightBigStrip.setPixelColor(p, color);
        rightSmallStrip.setPixelColor(p, color);
        p--;
        x++;
        showStrips();
        delay(wait);
    }
}


void stepColorReverse(uint8_t wait, uint16_t color, LPD8806 strip) {
    uint16_t i;
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
        strip.show();
        delay(wait);
    }
}

void stepColorMultipleStripsReverse(uint8_t wait, uint16_t color, LPD8806 strips[]) {
    uint16_t i;
    for (i = 0; i < BIG_STRAP_SIZE; i++) {
        for (int j = 0; j < 4; j++) {
            LPD8806 strip = strips[j];
            strip.setPixelColor(i, color);
            strip.show();
        }
        delay(wait);
    }
}

void animateMiddleReverse(uint8_t wait) {
    uint16_t i, p;
    p = BIG_STRAP_SIZE;
    for (i = 0; i <= MIDDLE_STRAP_SIZE; i++) {
        middleStrip.setPixelColor(i, Wheel(((p * 384 / middleStrip.numPixels())) % 384, middleStrip));
        p++;
        middleStrip.show();
        delay(wait);
    }
}

void animateStrapsReverse(uint8_t wait) {
    uint16_t i;
    for (i = 0; i <= BIG_STRAP_SIZE; i++) {
        leftSmallStrip.setPixelColor(i, Wheel(((i * 384 / leftSmallStrip.numPixels())) % 384, leftSmallStrip));
        leftBigStrip.setPixelColor(i, Wheel(((i * 384 / leftBigStrip.numPixels())) % 384, leftBigStrip));
        rightBigStrip.setPixelColor(i, Wheel(((i * 384 / rightBigStrip.numPixels())) % 384, rightBigStrip));
        rightSmallStrip.setPixelColor(i, Wheel(((i * 384 / rightSmallStrip.numPixels())) % 384, rightSmallStrip));
        showStrips();
        delay(wait);
    }
}

uint32_t Wheel(uint16_t WheelPos, LPD8806 strip) {
    byte r, g, b;
    switch (WheelPos / 128) {
        case 0:
            r = 127 - WheelPos % 128; // red down
            g = WheelPos % 128;       // green up
            b = 0;                    // blue off
            break;
        case 1:
            g = 127 - WheelPos % 128; // green down
            b = WheelPos % 128;       // blue up
            r = 0;                    // red off
            break;
        case 2:
            b = 127 - WheelPos % 128; // blue down
            r = WheelPos % 128;       // red up
            g = 0;                    // green off
            break;
        default:
            r = 255;
            g = 255;
            b = 255;
            break;

    }
    return (strip.Color(r, g, b));
}

void pulse(uint8_t wait) {

    for (int x = 0; x < 5; x++) {
        for (int j = 0; j < 256; j += 25) {
            for (int s = 0; s < 5; s++) {
                LPD8806 strip = allStrips[s];
                for (uint16_t i = 0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, colors[x] * gamma[j]);
                    strip.show();
                }
                delay(wait);
            }
        }


        for (int j = 255; j >= 0; j -= 25) {
            for (int s = 0; s < 5; s++) {
                LPD8806 strip = allStrips[s];
                for (uint16_t i = 0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, colors[x] * gamma[j]);
                    strip.show();
                }
                delay(wait);
            }
        }
    }
}
