#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG_PIN 10
#define DEBUG_SIZE 1
#define NUM_PIXELS 25
#define DATA_PIN 9
#define BOARD 8
// mess with this number to adjust TWINklitude :)
// lower number = more sensitive
#define MOVE_THRESHOLD 20

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel debug = Adafruit_NeoPixel(DEBUG_SIZE, DEBUG_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_LSM303 lsm;


Adafruit_NeoPixel board = Adafruit_NeoPixel(1, BOARD, NEO_GRB + NEO_KHZ800);

// Here is where you can put in your favorite colors that will appear!
// just add new {nnn, nnn, nnn}, lines. They will be picked out randomly
//                                  R   G   B
uint8_t myFavoriteColors[][3] = {{200, 0,   200},   // purple
                                 {200, 0,   0},   // red
                                 {0,   188, 212},   // cyan
                                 {0,   255, 0}, // green
};

// don't edit the line below
#define FAVCOLORS sizeof(myFavoriteColors) / 3

void flashRandom(int wait, uint8_t howmany);

void testDress();

void flashDress();

void setup() {
    Serial.begin(9600);

    // Try to initialise and warn if we couldn't detect the chip
//    if (!lsm.begin()) {
//        Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
//        while (1);
//    }

    debug.begin();
    debug.show();
    debug.setBrightness(95);

    board.begin();
    board.show(); // Initialize all pixels to 'off'
    board.setBrightness(60);

    strip.setBrightness(95);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

int counter = 0;

void loop() {
    testDress();
//    flashDress();
}

void testDress() {
    for (uint16_t i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
        strip.show();

        board.setPixelColor(0, board.Color(0, 255, 0));
        board.show();

        debug.setPixelColor(0, board.Color(0, 255, 0));
        debug.show();

        delay(100);

        board.setPixelColor(0, board.Color(0, 0, 0));
        board.show();

        debug.setPixelColor(0, board.Color(0, 0, 0));
        debug.show();

        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
        delay(100);
    }
}

void flashDress() {
    //  Take a reading of accellerometer data
    lsm.read();

    //  Get the magnitude (length) of the 3 axis vector
    //  http://en.wikipedia.org/wiki/Euclidean_vector#Length
    double storedVector = lsm.accelData.x * lsm.accelData.x;
    storedVector += lsm.accelData.y * lsm.accelData.y;
    storedVector += lsm.accelData.z * lsm.accelData.z;
    storedVector = sqrt(storedVector);

    //  get new data!
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