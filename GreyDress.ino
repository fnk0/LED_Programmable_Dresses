#include <Arduino.h>
#include "LPD8806.h"

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

// Function prototypes
void startStrips();

void showStrips();

void animateMiddle(uint8_t wait);

void animateStraps(uint8_t wait);

void animateMiddleReverse(uint8_t wait);

void animateStrapsReverse(uint8_t wait);

void colorWipe(uint32_t c, uint8_t wait, LPD8806 strip);

void colorWipeStraps(uint32_t c, uint8_t wait);

void colorWipeReverse(uint32_t c, uint8_t wait, LPD8806 strip);

void colorWipeStrapsReverse(uint32_t c, uint8_t wait);

uint32_t Wheel(uint16_t WheelPos, LPD8806 strip);

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

    animateStrapsReverse(DELAY);
    animateMiddleReverse(DELAY);

    for(int i = 0; i < 5; i++) {
        colorWipe(colors[i], DELAY, middleStrip);
        colorWipeStraps(colors[i], DELAY);
    }

//    colorWipeReverse(middleStrip.Color(0, 0, 0), DELAY, middleStrip);
//    colorWipeStrapsReverse(middleStrip.Color(0, 0, 0), DELAY);
//    stepColorMultipleStripsReverse(DELAY, leftBigStrip.Color(0, 0, 0), allStrips);
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

void colorWipe(uint32_t c, uint8_t wait, LPD8806 strip) {
    uint16_t i;
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorWipeReverse(uint32_t c, uint8_t wait, LPD8806 strip) {
    uint16_t i;
    for (i = strip.numPixels(); i >= 0; i--) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorWipeStraps(uint32_t c, uint8_t wait) {
    for(uint16_t i = 0; i < leftBigStrip.numPixels(); i++) {
        leftBigStrip.setPixelColor(i, c);
        rightBigStrip.setPixelColor(i, c);
        leftSmallStrip.setPixelColor(i, c);
        rightSmallStrip.setPixelColor(i, c);
        showStrips();
        delay(wait);
    }
}

void colorWipeStrapsReverse(uint32_t c, uint8_t wait) {
    for(uint16_t i = leftBigStrip.numPixels(); i >= 0; i--) {
        leftBigStrip.setPixelColor(i, c);
        rightBigStrip.setPixelColor(i, c);
        leftSmallStrip.setPixelColor(i, c);
        rightSmallStrip.setPixelColor(i, c);
        showStrips();
        delay(wait);
    }
}


void animateMiddle(uint8_t wait) {
    uint16_t i, j, p;
    p = MIDDLE_STRAP_SIZE;
    for (i = 0; i <= MIDDLE_STRAP_SIZE; i++) {
        uint32_t color = Wheel(((i * 384 / middleStrip.numPixels()) + j) % 384, middleStrip);
        middleStrip.setPixelColor(p, color);
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
