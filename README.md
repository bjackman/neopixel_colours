This repo probably isn't useful to anyone, I'm just writing this for future me.

I sewed some [Adafruit
Neopixels](https://learn.adafruit.com/adafruit-neopixel-uberguide/overview) onto
the collar of a boiler suit to wear at
[Shambala](http://shambalafestival.co.uk/) this is the code that makes them
flash and stuff. I built it with the [Arduino
Makefile](https://github.com/sudar/Arduino-Makefile) [2], you need the latest
version.

To get it to work with the platform I'm using (Adafruit Flora), I installed the
libraries and stuff via the official Arduino IDE's "board manager" [1], then set
up the Makefile to use those. env.mk has the system-specific stuff (for my
Ubuntu 16.04 machine) that makes that work.

[1] You need an up-to-date version of the IDE. Download it off the website,
    don't use a package.

[2] Same for this, clone it from the repo, don't use a package.
