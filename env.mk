# This bit is probably generic, the Arduino IDE created this ~/.arduino15
# directory, not me. I used the "board manager" to install the Flora stuff and
# then the files below appeared.
export BOARDS_TXT=$(HOME)/.arduino15/packages/adafruit/hardware/avr/1.4.9/boards.txt
export ARDUINO_VAR_PATH=$(HOME)/.arduino15/packages/adafruit/hardware/avr/1.4.9/variants/

# This bit is just the directories where I happened to install these things
export ARDUINO_DIR=/opt/arduino-1.6.10/
export ARDMK_DIR=~/Arduino-Makefile

# If your Arduino.mk is old, you'll need this to get compilation to work:
# export CXXFLAGS += -fno-threadsafe-statics # Get rid of __cxa_guard_acquire references
# export CXXFLAGS_STD = -std=c++11           # The libraries use `auto` which is C+11
