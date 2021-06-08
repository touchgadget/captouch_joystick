# USB Joystick with Touch Inputs

USB joystick with 12 cap touch inputs and 8 button inputs. Four of the touch
inputs are used for stick movement. Unused inputs can be left unconnected.

This program is similar to the joystick_captouch program but excludes the
analog joystick/thumbstick.

## Hardware

* Raspberry Pi Pico
* Adafruit MPR121 Gator Breakout https://www.adafruit.com/product/4830
* STEMMA QT/Qwicc JST cable with header pins https://www.adafruit.com/product/4209
* buttons for button inputs, optional
* conductive material for touch inputs

## Software

This project is based on the Raspberry Pi Pico using the Arduino IDE with the
following board package. See the web site for installation instructions.

https://github.com/earlephilhower/arduino-pico

The following Arduino libraries may be installed using the IDE Library Manager.

* Adafruit BusIO
* Adafruit MPR121
* Bounce2

## Hook up

### Touch Inputs

The 12 capacitive touch inputs are provided by an MPR121 board. The MPR121
board connects to the Pi Pico via I2C bus. The Adafruit MPR121 Gator board is
easy to use but a generic MPR121 board will also work.

MPR121  |Cable  |Raspberry Pi Pico
--------|-------|-----------------
3.3V    |Red    |3V3_OUT (pin 36)
GND     |Black  |GND (pin 3)
SCL     |Blue   |GP1, I2C0_SCL (pin 2)
SDA     |Yellow |GP0, I2C0_SDA (pin 1)

Touch   |Joystick
--------|--------
0       |Button 1
1       |Button 2
2       |Button 3
3       |Button 4
4       |Button 5
5       |Button 6
6       |Button 7
7       |Button 8
8       |Stick Forward
9       |Stick Backward
10      |Stick Left
11      |Stick Right

### Button Inputs

The following GPIO pins are configured as button inputs.

Button  |Raspberry Pi Pico
--------|-----------------
1       |GP4 (pin 6)
2       |GP5 (pin 7)
3       |GP6 (pin 9)
4       |GP7 (pin 10)
5       |GP8 (pin 11)
6       |GP9 (pin 12)
7       |GP10 (pin 14)
8       |GP11 (pin 15)
