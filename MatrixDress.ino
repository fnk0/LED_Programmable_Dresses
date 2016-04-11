#include <Arduino.h>
#include <SPI.h>
#include <Adafruit-GFX-Library/Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix/Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar/Adafruit_DotStar.h>
#include <FastLED/FastLED.h>

#define STRAP_SIZE 18
#define FEATHER_SIZE 32
#define MATRIX_SIZE 8

#define MATRIX  6
#define STRAP_RIGHT 12
#define STRAP_LEFT 10
#define FEATHER_RING 9
#define CLOCK 0

#define BRIGHTNESS  200
#define COLOR_ORDER GRB

#define DELAY 50

CRGB straps[STRAP_SIZE];
CRGB feather[FEATHER_SIZE];

uint8_t heart1[] = {
        0x00, 0x66, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00
};

uint8_t heart2[] = {
        0x00, 0x00, 0x24, 0x7e, 0x3c, 0x18, 0x18, 0x00
};

uint8_t heart3[] = {
        0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00
};

uint8_t heart4[] = {
        0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00
};

uint8_t *hearts[4] = {
        heart4, heart3, heart2, heart1
};

uint8_t pacLeft1[] = {
        0x38, 0x7c, 0xfe, 0x3f, 0x0f, 0x3f, 0xfe, 0x7c,
};

uint8_t pacLeft2[] = {
        0x38, 0x7c, 0xfe, 0xff, 0x1f, 0xff, 0xfe, 0x7c,
};

uint8_t pacLeft3[] = {
        0x38, 0x7c, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0x7c
};

uint8_t *pacLeft[3] = {
        pacLeft1, pacLeft2, pacLeft3
};

uint8_t pacRight1[] = {
        0x1c, 0x3e, 0x7f, 0xfc, 0xf0, 0xfc, 0x7f, 0x3e,
};

uint8_t pacRight2[] = {
        0x1c, 0x3e, 0x7f, 0xff, 0xf8, 0xff, 0x7f, 0x3e,
};

uint8_t pacRight3[] = {
        0x1c, 0x3e, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0x3e,
};

uint8_t *pacRight[3] = {
        pacRight1, pacRight2, pacRight3
};

uint8_t gLeft1[] = {
        0x1c, 0x3e, 0x5a, 0x7e, 0x7e, 0x7e, 0x76, 0x22,
};

uint8_t gLeft2[] = {
        0x1c, 0x3e, 0x5a, 0x7e, 0x7e, 0x7e, 0x3a, 0x10,
};

uint8_t gLeft3[] = {
        0x1c, 0x3e, 0x5a, 0x7e, 0x7e, 0x7e, 0x5c, 0x08,
};

uint8_t gLeft4[] = {
        0x1c, 0x3e, 0x5a, 0x7e, 0x7e, 0x7e, 0x6e, 0x44,
};

uint8_t *gLeft[4] = {
        gLeft1, gLeft2, gLeft3, gLeft4
};

uint8_t gRight1[] = {
        0x38, 0x7c, 0x56, 0x7e, 0x6a, 0x56, 0x6e, 0x44,
};

uint8_t gRight2[] = {
        0x38, 0x7c, 0x56, 0x7e, 0x56, 0x6a, 0x5c, 0x08,
};

uint8_t gRight3[] = {
        0x38, 0x7c, 0x56, 0x7e, 0x6a, 0x56, 0x3a, 0x10,
};

uint8_t gRight4[] = {
        0x38, 0x7c, 0x56, 0x7e, 0x56, 0x6a, 0x76, 0x22,
};

uint8_t *gRight[4] = {
        gRight1, gRight2, gRight3, gRight4
};



Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
        MATRIX_SIZE, MATRIX_SIZE, 1, 1, MATRIX, CLOCK,
        DS_TILE_TOP + DS_TILE_LEFT + DS_TILE_ROWS + DS_TILE_PROGRESSIVE +
        DS_MATRIX_TOP + DS_MATRIX_LEFT + DS_MATRIX_ROWS + DS_MATRIX_ZIGZAG,
        DOTSTAR_RGB);

void pulseHeart(CRGB color);
void doPacmanAnimation();
void fadeOut(CRGB color);
void fadeIn(CRGB color);
void drawRectangleFeather(CRGB color);
void pulseAnimation(CRGB color);
void drawFade(CRGB color, uint8_t i);
void drawBitmapsForFrame(int frame);
void drawGhost(int state, uint16_t color, int frame, int pos, int offset);
void setStrapColor(int frame);
void handleFramePos(int frame);
void fadeHeartOut(CRGB color);
void fadeHeartIn(CRGB color);
void drawHeart(CRGB color);
void rainbow_march();
CRGB getNextRainbowColor();
void cycleStraps(CRGB color, unsigned long timeDelay, int rainbow);
void cycleStrapsReverse(CRGB color, unsigned long timeDelay, int rainbow);
void cycleMatrixReverse(CRGB color, unsigned long timeDelay, int rainbow);
void cycleMatrix(CRGB color, unsigned long timeDelay, int rainbow);
void drawPixelForPosition(CRGB color, unsigned long timeDelay, int i, int rainbow);
void drawPixelForPositionReverse(CRGB color, unsigned long timeDelay, int i, int rainbow);

// Initialize global variables for sequences
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.
int globalState = 1;
int state = 0;
int loopCount = 0;
int heartCounter = 0;
uint8_t counter = 100;
uint16_t globalCounter = 0;
int reverseMode = 0;

void setup() {
    matrix.begin();
    matrix.clear();
    matrix.show();
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.setTextWrap(false);
    matrix.setBrightness(BRIGHTNESS);
    matrix.setRotation(135);

    FastLED.addLeds<NEOPIXEL, FEATHER_RING>(feather, FEATHER_SIZE);
    FastLED.addLeds<LPD8806, STRAP_LEFT, CLOCK, COLOR_ORDER>(straps, STRAP_SIZE);
    FastLED.addLeds<LPD8806, STRAP_RIGHT, CLOCK, COLOR_ORDER>(straps, STRAP_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

    for(int i = 0; i < 12; i++) {

        switch (i) {
            case 3:
                matrix.clear();
                doPacmanAnimation();
                break;
            case 7:
                cycleStraps(CRGB::Black, 0, 0);
                for(int k = 0; k < 50; k++) {
                    drawHeart(CRGB::Red);
                }
                break;
            case 2:
            case 6:
            case 9:
            case 11:
                pulseAnimation(ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND));
                counter+= 100;
                break;
            default:
                FastLED.setBrightness(BRIGHTNESS);
                matrix.setBrightness(BRIGHTNESS);
                rainbow_march();
                delay(10);
                break;
        }
    }

    matrix.clear();
    FastLED.setBrightness(BRIGHTNESS);
    matrix.setBrightness(BRIGHTNESS);
}

void rainbow_march() {
    cycleStraps(CRGB::Black, 5, 1);
    cycleMatrix(CRGB::Black, 5, 1);
    cycleMatrixReverse(CRGB::Black, 5, 1);
    cycleStrapsReverse(CRGB::Black, 5, 1);
}

void cycleMatrix(CRGB color, unsigned long timeDelay, int rainbow) {
    for(int i = 0; i < 8; i++) {
        CRGB nc = rainbow == 0 ? color : ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND);
        for(int j = 0; j < 8; j++) {
            matrix.drawPixel(j, i, matrix.Color(nc.b, nc.g, nc.r));
        }
        matrix.show();
        delay(timeDelay);
        counter+= 5;
    }
}

void cycleMatrixReverse(CRGB color, unsigned long timeDelay, int rainbow) {
    for(int i = 8; i >= 0; i--) {
        CRGB nc = rainbow == 0 ? color : ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND);
        for(int j = 8; j >= 0; j--) {
            matrix.drawPixel(j, i, matrix.Color(nc.b, nc.g, nc.r));
        }
        matrix.show();
        delay(timeDelay);
        counter+= 5;
    }
}

void pulseAnimation(CRGB color) {
    matrix.drawRect(0, 0, 8, 8, matrix.Color(color.b, color.g, color.r));
    matrix.drawRect(2, 2, 4, 4, matrix.Color(color.b, color.g, color.r));
    fadeIn(color);
    fadeOut(color);
}

void pulseHeart(CRGB color) {
    fadeHeartIn(color);
    fadeHeartOut(color);
}

void doPacmanAnimation() {
    cycleStraps(CRGB::Yellow, 50, 0);
    state = 0;
    for (int i = -9; i < 40; i++) {
        handleFramePos(i);
    }
    state = 1;
    for (int i = 40; i >= -9; i--) {
        handleFramePos(i);
    }
    cycleStrapsReverse(CRGB::Black, 50, 0);
}

void handleFramePos(int frame) {
    setStrapColor(frame);
    drawBitmapsForFrame(frame);
    matrix.show();
    delay(DELAY);
}

void setStrapColor(int frame) {

    if (state == 0) {
        if (frame < 7) {
            fill_solid(straps, STRAP_SIZE, CRGB::Yellow);
            fill_solid(feather, FEATHER_SIZE, CRGB::Yellow);
        } else if (frame < 15) {
            fill_solid(straps, STRAP_SIZE, CRGB::Red);
            fill_solid(feather, FEATHER_SIZE, CRGB::Red);
        } else if (frame < 23) {
            fill_solid(straps, STRAP_SIZE, CRGB::DeepPink);
            fill_solid(feather, FEATHER_SIZE, CRGB::DeepPink);
        } else if (frame < 31) {
            fill_solid(straps, STRAP_SIZE, CRGB::Aqua);
            fill_solid(feather, FEATHER_SIZE, CRGB::Aqua);
        } else {
            fill_solid(straps, STRAP_SIZE, CRGB::Orange);
            fill_solid(feather, FEATHER_SIZE, CRGB::Orange);
        }
    } else {
        if (frame > 8) {
            fill_solid(straps, STRAP_SIZE, CRGB::Blue);
            fill_solid(feather, FEATHER_SIZE, CRGB::Blue);
        } else {
            fill_solid(straps, STRAP_SIZE, CRGB::Yellow);
            fill_solid(feather, FEATHER_SIZE, CRGB::Yellow);
        }
    }

    FastLED.show();
}

void drawBitmapsForFrame(int frame) {
    matrix.fillScreen(0);
    int pos = abs(frame % 2);

    // Draw pacman running
    if (state == 0) {
        matrix.drawBitmap(frame, 0, pacRight[pos], 8, 8, matrix.Color(59, 255, 255));
        drawGhost(0, matrix.Color(0, 0, 255), frame, pos, 8);
        drawGhost(0, matrix.Color(255, 0, 255), frame, pos, 16);
        drawGhost(0, matrix.Color(255, 255, 50), frame, pos, 24);
        drawGhost(0, matrix.Color(77, 183, 255), frame, pos, 32);
    } else {
        drawGhost(1, matrix.Color(0, 0, 255), frame, pos, 8);
        drawGhost(1, matrix.Color(255, 0, 255), frame, pos, 16);
        drawGhost(1, matrix.Color(255, 255, 50), frame, pos, 24);
        drawGhost(1, matrix.Color(77, 183, 255), frame, pos, 32);
        matrix.drawBitmap(frame, 0, pacLeft[pos], 8, 8, matrix.Color(59, 255, 255));
    }

}

void drawGhost(int state, uint16_t color, int frame, int pos, int offset) {
    int ghostPos = frame - offset;
    if (state == 0) {
        matrix.drawBitmap(ghostPos, 0, gLeft[pos], 8, 8, color);
        matrix.drawPixel(ghostPos + 2, 2, matrix.Color(255, 255, 255));
        matrix.drawPixel(ghostPos + 3, 2, matrix.Color(255, 0, 0));
        matrix.drawPixel(ghostPos + 5, 2, matrix.Color(255, 255, 255));
        matrix.drawPixel(ghostPos + 6, 2, matrix.Color(255, 0, 0));
    } else {
        matrix.drawBitmap(ghostPos, 0, gRight[pos], 8, 8, matrix.Color(255, 0, 0));
    }
}

void drawHeart(CRGB color) {
    matrix.clear();
    matrix.show();
    if (heartCounter == 3) {
        globalState = 1;
    }
    if (heartCounter == 0) {
        globalState = 0;
    }

    if(globalState == 1) {
        heartCounter--;
    } else {
        heartCounter++;
    }

    matrix.drawBitmap(0, 0, hearts[heartCounter], 8, 8, matrix.Color(color.b, color.g, color.r));
    matrix.show();

    int position = loopCount % STRAP_SIZE - 2;
    if (reverseMode == 0) {
        drawPixelForPosition(color, 0, position, 0);
    } else {
        drawPixelForPositionReverse(color, 0, (STRAP_SIZE - 2) - position, 0);
    }

    loopCount++;
    delay(50);
//    if (reverseMode == 0) {
//        drawPixelForPosition(CRGB::Black, 10, position, 0);
//    }

    if (loopCount % (STRAP_SIZE - 2) == 0) {
        reverseMode = 1;
    }
}

void drawPixelForPosition(CRGB color, unsigned long timeDelay, int i, int rainbow) {
    if (i == 12) {
        CRGB nc = rainbow == 0 ? color : getNextRainbowColor();
        for (int j = 0; j < FEATHER_SIZE; j++) {
            feather[j] = nc;
            if (j % 7 == 0 && j != 0) {
                FastLED.show();
                delay(timeDelay);
            }
        }
    }
    CRGB nc = rainbow == 0 ? color : getNextRainbowColor();
    straps[i] = nc;
    if (i == 12) {
        straps[16] = nc;
    }
    if (i == 13) {
        straps[17] = nc;
    }
    if (rainbow) {
        counter += 5;
    }
    FastLED.show();
}

void drawPixelForPositionReverse(CRGB color, unsigned long timeDelay, int i, int rainbow) {
    if (i == 11) {
        CRGB nc = rainbow == 0 ? color : getNextRainbowColor();
        for (int j = FEATHER_SIZE; j >= 0; j--) {
            feather[j] = nc;
            if (j % 7 == 0 && j != 0) {
                FastLED.show();
                delay(timeDelay);
            }
        }
    }
    CRGB nc = rainbow == 0 ? color : getNextRainbowColor();
    straps[i] = nc;
    if (i == 13) {
        straps[16] = nc;
    }
    if (i == 12) {
        straps[17] = nc;
    }

    if (rainbow) {
        counter += 5;
    }
    FastLED.show();
}

void cycleStraps(CRGB color, unsigned long timeDelay, int rainbow) {
    for (int i = 0; i < STRAP_SIZE - 2; i++) {
        drawPixelForPosition(color, timeDelay, i, rainbow);
        delay(timeDelay);
    }
}

void cycleStrapsReverse(CRGB color, unsigned long timeDelay, int rainbow) {
    for (int i = STRAP_SIZE - 2; i >= 0; i--) {
        drawPixelForPositionReverse(color, timeDelay, i, rainbow);
        delay(timeDelay);
    }
}


void fadeHeartIn(CRGB color) {
    for (uint8_t i = 0; i <= BRIGHTNESS; i++) {
        if(i % 35    == 0) {
            drawHeart(color);
        }
        drawFade(color, i);
    }
}

void fadeHeartOut(CRGB color) {
    for (uint8_t i = BRIGHTNESS; i > 0; i--) {
        drawFade(color, i);
    }
}

void fadeIn(CRGB color) {
    for (uint8_t i = 0; i <= BRIGHTNESS; i++) {
        drawFade(color, i);
    }
}

void fadeOut(CRGB color) {
    for (uint8_t i = BRIGHTNESS; i > 0; i--) {
        drawFade(color, i);
    }
}

void drawFade(CRGB color, uint8_t i) {
    FastLED.setBrightness(i);
    matrix.setBrightness(i);
    fill_solid(straps, STRAP_SIZE, color);
    drawRectangleFeather(color);
    matrix.show();
    FastLED.show();
}

void drawRectangleFeather(CRGB color) {
    for(int j = 0; j < FEATHER_SIZE; j++) {
        if (j < 8 || j > 24) {
            feather[j] = color;
        } else {
            if (j % 8 == 0
                || j == 7
                || j == 15
                || j == 23
                || j == 31) {
                feather[j] = color;
            }
        }
    }
}

CRGB getNextRainbowColor() {
    return ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND);
}