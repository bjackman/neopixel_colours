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

#define TABLE_SIZE ARRAY_SIZE(sinTable)
#define TABLE_THIRD (TABLE_SIZE / 3)

#define BRIGHTNESS_DIVIDER 16
#define INDEX_TO_COLOURVAL(i, thirds) \
  (sinTable[(index + TABLE_THIRD * thirds) % TABLE_SIZE] / BRIGHTNESS_DIVIDER)

uint32_t rgbCircle(int index) {
  return pixels.Color(INDEX_TO_COLOURVAL(index, 0),
                      INDEX_TO_COLOURVAL(index, 1),
                      INDEX_TO_COLOURVAL(index, 2));
}

void fadeColours() {
  int indices[NUM_PIXELS];

  for (int i = 0; i < NUM_PIXELS; i++)
    indices[i] = i * 13;

  for(;;){

    for (int p = 0; p < NUM_PIXELS; p++) {
      uint32_t colour = rgbCircle(indices[p]++);
      myPrintf("%08x\r\n", colour);
      pixels.setPixelColor(p, colour);
    }

    myPrintf("\r\n");

    pixels.show();

    delay(delayMs);

  }
}

void loop() {
  fadeColours();
}

void flashRgb() {
  Serial.println("switching on");

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 255));

  pixels.show();

  delay(300);

  Serial.println("switching off");

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));

  pixels.show();

  delay(300);

}
