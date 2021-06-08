# USB Joystick with Capacitive Touch Inputs

![Pi Pico with thumbstick and touch input board](./joystick_captouch/images/joystick_captouch.jpg)

The captouch_joystick project turns a Raspberry Pi Pico into a USB joystick
with 16 bit X and Y axes and 12 capacitive touch inputs in addition to
button inputs. The hardware consists of a Raspberry Pi Pico with an analog
joystick or thumbstick and an MPR121 break out board. The MPR121 provides 12
capacitive touch inputs.

captouch_joystick works as a generic USB HID joystick and with the XBox
Adaptive Controller.

joystick_captouch/ creates a joystick with one analog joystick/thumbstick and
12 touch inputs for buttons.

joystick_alltouch/ creates a joystick with 8 touch inputs for joystick buttons
and 4 touch inputs for joystick movement instead of an analog joystick.
