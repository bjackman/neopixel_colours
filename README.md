This repo probably isn't useful to anyone, I'm just writing this for future me.

I sewed some [Adafruit
Neopixels](https://learn.adafruit.com/adafruit-neopixel-uberguide/overview) onto
the collar of a boiler suit to wear at
[Shambala](http://www.shambalafestival.org/), this is the code that makes them
flash and stuff. I built it with the [Arduino
Makefile](https://github.com/sudar/Arduino-Makefile) [2], you need the latest
version.

To get it to work with the platform I'm using (Adafruit Flora), I installed the
libraries and stuff via the official Arduino IDE's "board manager" [1], then set
up the Makefile to use those. env.mk has the system-specific stuff (for my
Ubuntu 16.04 machine) that makes that work.

Get the Neopixel library with [these instructions](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation),
and the Flora board support stuff with [these ones](https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide). I used 
- Arduino IDE 1.6.10
- NeoPixel library 1.0.6
- [Arduino Makefile 1.5.1-9-g0ec1489](https://github.com/sudar/Arduino-Makefile/tree/0ec1489414f0bd7fbc9569d334984ef737c9f2a9)

[1] You need an up-to-date version of the IDE. Download it off the website,
    don't use a package.

[2] Same for this, clone it from the repo, don't use a package.
