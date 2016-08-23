#include <stdarg.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            1

#define NUM_PIXELS      6
#define LAST_PIXEL (NUM_PIXELS - 1)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

#ifdef MY_PRINTF_ENABLED
void myPrintf(const char *fmt, ... ){
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
  pixels.begin();
}

int sinTable[] = {
  127,  135,  143,  151,  159,  166,  174,  181,  188,  195,  202,  208,
  214,  220,  225,  230,  235,  239,  242,  246,  248,  250,  252,  253,
  254,  255,  254,  253,  252,  250,  248,  246,  242,  239,  235,  230,
  225,  220,  214,  208,  202,  195,  188,  181,  174,  166,  159,  151,
  143,  135,  127,  119,  111,  103,   95,   88,   80,   73,   66,   59,
   52,   46,   40,   34,   29,   24,   19,   15,   12,    8,    6,    4,
    2,    1,    0,    0,    0,    1,    2,    4,    6,    8,   12,   15,
   19,   24,   29,   34,   40,   46,   52,   59,   66,   73,   80,   88,
   95,  103,  111,  119,
};

#define ROUND_DOWN(val, multiplier) (val - (val % multiplier))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define TABLE_SIZE ARRAY_SIZE(sinTable)
#define TABLE_THIRD (TABLE_SIZE / 3)

#define BRIGHTNESS_DIVIDER 8
#define MAX_BRIGHTNESS (255 / BRIGHTNESS_DIVIDER)
#define INDEX_TO_COLOURVAL(i, thirds) \
  (sinTable[(index + TABLE_THIRD * thirds) % TABLE_SIZE] / BRIGHTNESS_DIVIDER)

void setAllToColour(uint32_t colour) {
    for (unsigned int i = 0; i < NUM_PIXELS; i++)
      pixels.setPixelColor(i, colour);
}


uint32_t rgbCircle(int index) {
  return pixels.Color(INDEX_TO_COLOURVAL(index, 0),
                      INDEX_TO_COLOURVAL(index, 1),
                      INDEX_TO_COLOURVAL(index, 2));
}

// Circle the pixels through the edge of RGB colour space (I think?). Each pixel
// is at a slight offset from its neighbour.
void fadeColours(unsigned int time) {
  int indices[NUM_PIXELS];
  int delayMs = 10;

  time = ROUND_DOWN(time, delayMs);

  for (int i = 0; i < NUM_PIXELS; i++)
    indices[i] = i * 7;

  while ((time -= delayMs)) {

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

uint32_t red   = pixels.Color(MAX_BRIGHTNESS, 0, 0);
uint32_t green = pixels.Color(0, MAX_BRIGHTNESS, 0);
uint32_t blue  = pixels.Color(0, 0, MAX_BRIGHTNESS);
uint32_t off   = pixels.Color(0, 0, 0);
uint32_t white = pixels.Color(MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS);

void flashRgb(unsigned int time) {
  uint32_t colours[] = {red, green, blue};
  int delayMs = 300;

  time = ROUND_DOWN(time, delayMs);

  while ((time -= delayMs)) {
    myPrintf("switching on");
    for (int i = 0; i < NUM_PIXELS; i++)
      pixels.setPixelColor(i, colours[i % ARRAY_SIZE(colours)]);

    pixels.show();
    delay(300);

    myPrintf("switching off");
    setAllToColour(off);

    pixels.show();
    delay(300);
  }
}


// Zip back and forth along the pixels, circulating colour as we go
void pingPong(unsigned int time) {
  int pixel = 0, prevPixel = 0;
  int nextOffset = 1;
  int delayMs = 50;
  int colourIndex = 0;

  time = ROUND_DOWN(time, delayMs);

  while ((time -= delayMs)) {
    pixels.setPixelColor(prevPixel, off);
    pixels.setPixelColor(pixel, rgbCircle(colourIndex));

    pixels.show();

    colourIndex += 1;

    prevPixel = pixel;
    pixel += nextOffset;

    if ((pixel == LAST_PIXEL) || (pixel == 0))
      nextOffset *= -1;

    delay(delayMs);
  }
}

uint32_t randomColour() {
  return pixels.Color(random(0, MAX_BRIGHTNESS),
		      random(0, MAX_BRIGHTNESS),
		      random(0, MAX_BRIGHTNESS));
}

uint32_t randomOrOff() {
  if (random(100) > 40)
    return randomColour();
  else
    return off;
}

void doRand(unsigned int time, uint32_t (*randFunc)(void)) {
  int delayMs = 90;

  time = ROUND_DOWN(time, delayMs);

  while (time -= delayMs) {
    for (unsigned int i = 0; i < NUM_PIXELS; i++)
      pixels.setPixelColor(i, randFunc());

    pixels.show();

    delay(delayMs);
  }
}

// Make each pixel a random colour
void randomColours(unsigned int time) {
  doRand(time, randomColour);
}

// Make each pixel a random colour, sometimes off
void bleepBloop(unsigned int time) {
  doRand(time, randomOrOff);
}

void doFlashing(unsigned int time, uint32_t *colours, unsigned int numColours) {
  int colourIndex = 1;
  int delayMs = 40; // yeah we flashin
                    // would like to go faster but it just starts to be PWM

  time = ROUND_DOWN(time / 2, delayMs);

  while (time -= delayMs) {
    uint32_t colour = colours[colourIndex];

    setAllToColour(colour);

    pixels.show();
    delay(delayMs);

    setAllToColour(off);

    pixels.show();
    colourIndex = (colourIndex + 1) % numColours;;
    delay(delayMs);
  }
}

// This function is named in caps because it's very intense and cares nothing
// for your coding style
void THIS_AINT_NO_MACRO(unsigned int time) {
  doFlashing(time, &white, 1);
}

void flashCircle(unsigned int time) {
  uint32_t colours[30];

  for (unsigned int i = 0; i < ARRAY_SIZE(colours); i++)
    colours[i] = rgbCircle(i * 10);

  doFlashing(time, colours, ARRAY_SIZE(colours));
}

typedef void (*pixel_func_t)(unsigned int time);

pixel_func_t funcs[] = {
  flashCircle, THIS_AINT_NO_MACRO, bleepBloop, randomColours, pingPong, fadeColours,
  // flashRgb, This one is shitty
};

void loop() {
  for (unsigned int i = 0; i < ARRAY_SIZE(funcs); i++) {
    pixel_func_t func = funcs[i % ARRAY_SIZE(funcs)];
    func(5000);
  }
}
