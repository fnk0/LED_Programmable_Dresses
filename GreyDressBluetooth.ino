#include "FastLED/FastLED.h"
/*********************************************************************
  This is an example for our nRF51822 based Bluefruit LE modules

  Pick one up today in the adafruit shop!

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
*********************************************************************/

#include <string.h>
#include <Arduino.h>
#include <SPI.h>

#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
#include <SoftwareSerial.h>
#endif

#include "Adafruit_BluefruitLE_nRF51/Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_nRF51/Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_nRF51/Adafruit_BluefruitLE_UART.h"

#include "Bluefruit.h"

/*=========================================================================
    APPLICATION SETTINGS

      FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
     
                                Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                                running this at least once is a good idea.
     
                                When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                                Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
         
                                Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
  SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

  Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper *err) {
    Serial.println(err);
    while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);

float parsefloat(uint8_t *buffer);

void printHex(const uint8_t *data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];



/**************************************************************************/
/*!
    MY DEFINITIONS AND STUFF
*/
/**************************************************************************/

#define BRIGHTNESS  200
#define COLOR_ORDER GRB

#define SMALL_STRAP_SIZE 16
#define BIG_STRAP_SIZE 20
#define MIDDLE_STRAP_SIZE 18

#define MIDDLE_SHOULDER_SIZE 38
#define TOTAL_PIXELS 20

#define CLOCK 0

#define SMALL_LEFT 10
#define BIG_LEFT 1
#define SMALL_RIGHT 9
#define BIG_RIGHT 6
#define MIDDLE 12

#define DELAY 10
#define DELAY_PULSE 1

#define SOLID_COUNTER 25
#define GLOBAL_COUNTER 15

CRGB smallStrap[SMALL_STRAP_SIZE];
CRGB bigStrap[BIG_STRAP_SIZE];
CRGB middleStrap[MIDDLE_STRAP_SIZE];

void cycle(int rainbow);

void animateDress();
void cycleReverse(int rainbow);

void fadeIn(int rainbow, CRGB color);

void fadeOut(int rainbow, CRGB color);

void cycleFromCenter(int rainbow, CRGB color);

void cycleReverseToCenter(int rainbow, CRGB color);

void fillRainbow();

void fillSolid(CRGB color);
void myDelay(unsigned long d);

uint8_t thishue = 0;
uint8_t deltahue = 10;

uint8_t centerCounter = 0;
uint8_t reverseCenterCounter = 0;

uint8_t counter = 0;
uint8_t solidCounter = 0;

int loopCounter = 0;

CRGB rainbowColors[MIDDLE_SHOULDER_SIZE];

int globalCounter = 0;
int animations[8] = {0, 1, 2, 4, 4, 3, 0, 1};

int runAnimations = 1;



// Timer0 is already used for millis() - we'll just interrupt somewhere
// in the middle and call the "Compare A" function below

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void) {

    delay(500);

//    OCR0A = 0xAF;
//    TIMSK0 |= _BV(OCIE0A);

    FastLED.addLeds<LPD8806, SMALL_LEFT, CLOCK, COLOR_ORDER>(smallStrap, SMALL_STRAP_SIZE);
    FastLED.addLeds<LPD8806, SMALL_RIGHT, CLOCK, COLOR_ORDER>(smallStrap, SMALL_STRAP_SIZE);
    FastLED.addLeds<LPD8806, BIG_LEFT, CLOCK, COLOR_ORDER>(bigStrap, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, BIG_RIGHT, CLOCK, COLOR_ORDER>(bigStrap, BIG_STRAP_SIZE);
    FastLED.addLeds<LPD8806, MIDDLE, CLOCK, COLOR_ORDER>(middleStrap, MIDDLE_STRAP_SIZE);

    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    Serial.begin(115200);
    Serial.println(F("Adafruit Bluefruit App Controller Example"));
    Serial.println(F("-----------------------------------------"));

    /* Initialise the module */
    Serial.print(F("Initialising the Bluefruit LE module: "));

    if (!ble.begin(VERBOSE_MODE)) {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }
    Serial.println(F("OK!"));

    if (FACTORYRESET_ENABLE) {
        /* Perform a factory reset to make sure everything is in a known state */
        Serial.println(F("Performing a factory reset: "));
        if (!ble.factoryReset()) {
            error(F("Couldn't factory reset"));
        }
    }

    /* Disable command echo from Bluefruit */
    ble.echo(false);

    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    ble.info();

    Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
    Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
    Serial.println();

    ble.verbose(false);  // debug info is a little annoying after this point!

    /* Wait for connection */
//    while (!ble.isConnected()) {
//        delay(500);
//    }

    Serial.println(F("******************************"));

    // LED Activity command is only supported from 0.6.6
    if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION)) {
        // Change Mode LED Activity
        Serial.println(F("Change LED activity to "
                                 MODE_LED_BEHAVIOUR));
        ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    }

    // Set Bluefruit to DATA mode
    Serial.println(F("Switching to DATA mode!"));
    ble.setMode(BLUEFRUIT_MODE_DATA);

    Serial.println(F("******************************"));

    uint8_t count = 0;

    for (int i = 0; i < MIDDLE_SHOULDER_SIZE; i++) {
        rainbowColors[i] = ColorFromPalette(RainbowColors_p, count, 255, LINEARBLEND);
        count += GLOBAL_COUNTER / 2;
    }
}


void animateDress() {
    if (runAnimations == 1) {
        int pos = globalCounter % 8;
        int animation = animations[pos];

        switch (animation) {
            case 0:
                cycleFromCenter(1, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
//                cycle(1);
                cycleReverse(1);
                break;
            case 1:
                fadeOut(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
                solidCounter += SOLID_COUNTER;
                fadeIn(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
                solidCounter += SOLID_COUNTER;
                break;
            case 2:
                cycleFromCenter(1, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
                solidCounter += SOLID_COUNTER;
                cycleFromCenter(0, CRGB::Black);
                break;
            case 3:
                cycleReverseToCenter(0, ColorFromPalette(RainbowColors_p, solidCounter, 255, LINEARBLEND));
                solidCounter += SOLID_COUNTER;
                cycleReverseToCenter(0, CRGB::Black);
                break;
            case 4:
                fillRainbow();
                break;
            default:
                fillRainbow();
                break;
        }
        globalCounter++;
    } else {
        globalCounter = 0;
    }
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void) {
    /*
     * Read the data from the bluetooth
     */

    if(!ble.isConnected() || runAnimations == 1) {
        animateDress();
        return;
    }

    uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);

    Serial.print("Counter: ");
    Serial.print(globalCounter);
    Serial.print(" -    Len:");
    Serial.println(len);

    if (len == 0) {
        return;
    }
    runAnimations = 0;
    /* Got a packet! */
    printHex(packetbuffer, len);

    // Color
    if (packetbuffer[1] == 'C') {
        uint8_t red = packetbuffer[2];
        uint8_t green = packetbuffer[3];
        uint8_t blue = packetbuffer[4];

        Serial.print("VALUE: ");
        Serial.println(red);
        Serial.println(green);
        Serial.println(blue);

        Serial.print("RGB #");

        char concat_string[8];

        sprintf(concat_string, "0x%X%X%X", red, green, blue);

        if (red < 0x10) Serial.print("0");
        Serial.print(red, HEX);

        if (green < 0x10) Serial.print("0");
        Serial.print(green, HEX);


        if (blue < 0x10) Serial.print("0");
        Serial.println(blue, HEX);

        Serial.print("Concat String: ");
        Serial.println(concat_string);

        CRGB color = CRGB(red, green, blue); //(int) strtol(concat_string, NULL, 0);

        Serial.print("CRGB: ");
        Serial.print(" -- Red: ");
        Serial.print(color.r);
        Serial.print(" -- Green: ");
        Serial.print(color.g);
        Serial.print(" -- Blue: ");
        Serial.println(color.b);

        fillSolid(color);
        FastLED.show();

        if (color.r == 0 && color.g == 0 && color.b == 0) {
            runAnimations = 1;
        }
    }

    // Buttons
    if (packetbuffer[1] == 'B') {
        uint8_t buttnum = packetbuffer[2] - '0';
        boolean pressed = packetbuffer[3] - '0';
        Serial.print("Button ");
        Serial.print(buttnum);
        if (pressed) {
            Serial.println(" pressed");
        } else {
            Serial.println(" released");
        }
    }

    // GPS Location
    if (packetbuffer[1] == 'L') {
        float lat, lon, alt;
        lat = parsefloat(packetbuffer + 2);
        lon = parsefloat(packetbuffer + 6);
        alt = parsefloat(packetbuffer + 10);
        Serial.print("GPS Location\t");
        Serial.print("Lat: ");
        Serial.print(lat, 4); // 4 digits of precision!
        Serial.print('\t');
        Serial.print("Lon: ");
        Serial.print(lon, 4); // 4 digits of precision!
        Serial.print('\t');
        Serial.print(alt, 4);
        Serial.println(" meters");
    }

    // Accelerometer
    if (packetbuffer[1] == 'A') {
        float x, y, z;
        x = parsefloat(packetbuffer + 2);
        y = parsefloat(packetbuffer + 6);
        z = parsefloat(packetbuffer + 10);
        Serial.print("Accel\t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    // Magnetometer
    if (packetbuffer[1] == 'M') {
        float x, y, z;
        x = parsefloat(packetbuffer + 2);
        y = parsefloat(packetbuffer + 6);
        z = parsefloat(packetbuffer + 10);
        Serial.print("Mag\t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    // Gyroscope
    if (packetbuffer[1] == 'G') {
        float x, y, z;
        x = parsefloat(packetbuffer + 2);
        y = parsefloat(packetbuffer + 6);
        z = parsefloat(packetbuffer + 10);
        Serial.print("Gyro\t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    // Quaternions
    if (packetbuffer[1] == 'Q') {
        float x, y, z, w;
        x = parsefloat(packetbuffer + 2);
        y = parsefloat(packetbuffer + 6);
        z = parsefloat(packetbuffer + 10);
        w = parsefloat(packetbuffer + 14);
        Serial.print("Quat\t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.print('\t');
        Serial.print(w);
        Serial.println();

    }
}

void cycle(int rainbow) {

    for (int i = MIDDLE_STRAP_SIZE; i >= 0; i--) {
        middleStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        myDelay(DELAY);
    }

    for (int i = BIG_STRAP_SIZE; i >= 0; i--) {
        if (i < 18) {
            smallStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        }
        bigStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        myDelay(DELAY);
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
        myDelay(DELAY);
    }

    for (int i = 0; i < MIDDLE_STRAP_SIZE; i++) {
        middleStrap[i] = rainbow == 1 ? ColorFromPalette(RainbowColors_p, counter, 255, LINEARBLEND) : CRGB::Black;
        counter += GLOBAL_COUNTER;
        FastLED.show();
        myDelay(DELAY);
    }
}

void cycleFromCenter(int rainbow, CRGB color) {

    for (int i = 0; i < TOTAL_PIXELS; i++) {

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

            bigStrap[BIG_STRAP_SIZE - i - 1] = color;
        }

        FastLED.show();
        myDelay(DELAY);
    }
}

void cycleReverseToCenter(int rainbow, CRGB color) {

    for (int i = 0; i < TOTAL_PIXELS; i++) {
        if (i < SMALL_STRAP_SIZE) {
            smallStrap[i] = color;
        }

        if (i <= MIDDLE_STRAP_SIZE) {
            middleStrap[MIDDLE_STRAP_SIZE - i] = color;
        }

        bigStrap[i] = color;
        FastLED.show();
        myDelay(DELAY);
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

void myDelay(unsigned long d) {
    delay(d);
    digitalWrite(7, HIGH);
    digitalWrite(7, LOW);
}

void fillRainbow() {
    thishue++;
    fill_rainbow(smallStrap, SMALL_STRAP_SIZE, thishue, deltahue);
    fill_rainbow(bigStrap, BIG_STRAP_SIZE, thishue, deltahue);
    fill_rainbow(middleStrap, MIDDLE_STRAP_SIZE, thishue, deltahue);
}

void fillSolid(CRGB color) {
    fill_solid(smallStrap, SMALL_STRAP_SIZE, color);
    fill_solid(bigStrap, BIG_STRAP_SIZE, color);
    fill_solid(middleStrap, MIDDLE_STRAP_SIZE, color);
}
