#include <stdarg.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            1

// How many NeoPixels are attached to the Arduino?
#define NUM_PIXELS      3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayMs = 10;

#ifdef MY_PRINTF_ENABLED
void myPrintf(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
}
#else
#define myPrintf(...)
#endif

void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

int sinTable[] = {
  127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208,
  214, 220, 225, 230, 235, 239, 242, 246, 248, 250, 252, 253,
  254, 255, 254, 253, 252, 250, 248, 246, 242, 239, 235, 230,
  225, 220, 214, 208, 202, 195, 188, 181, 174, 166, 159, 151,
  143, 135, 127, 119, 111, 103, 95, 88, 80, 73, 66, 59, 52,
  46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 4, 2, 1, 0, 0, 0, 1,
  2, 4, 6, 8, 12, 15, 19, 24, 29, 34, 40, 46, 52, 59, 66, 73,
  80, 88, 95, 103, 111, 119
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

void loop() {
  int colours[3] = { 0 };
  int tableSize = ARRAY_SIZE(sinTable);
  int indexes[3] = {0, tableSize / 3, (tableSize / 3) * 2};

  for(int i=0;; i++){

    for (int c = 0; c < 3; c++) {
      colours[c] = sinTable[indexes[c]] / 8;
      //myPrintf("sinTable[%d] = %d so %d\n", indexes[c], sinTable[indexes[c]], colours[c]);
      indexes[c] = (indexes[c] + 1) % tableSize;
    }

    Serial.println();

    for (int p = 0; p < NUM_PIXELS; p++) {
      pixels.setPixelColor(p, pixels.Color(colours[0], colours[1], colours[2])); // Moderately bright green color.
    }

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayMs); // Delay for a period of time (in milliseconds).

  }
}
