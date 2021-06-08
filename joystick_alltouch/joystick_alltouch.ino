/************************************************************************
  MIT License

  Copyright (c) 2021 touchgadgetdev@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 *************************************************************************/

/*
 * 12 cap touch inputs
 * 0-7 joystick buttons 0-7
 * 8  joystick up
 * 9  joystick down
 * 10 joystick left
 * 11 joystick right
 *
 * 8 GPIO pins also can be used to activate joystick buttons
 */

#include <Bounce2.h>
#include "captouch_joystick.h"
#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 touch = Adafruit_MPR121();

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {4, 5, 6, 7, 8, 9, 10, 11};

Bounce * buttons = new Bounce[NUM_BUTTONS];

TUJoystick Joystick;

bool USE_TOUCH = true;

void setup()
{
  Joystick.begin();

  Serial.begin(115200);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i], INPUT_PULLUP );
    buttons[i].interval(5);
  }

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  if (!touch.begin(0x5A)) {
    Serial.println("MPR121 not found");
    USE_TOUCH = false;
  }

  // wait until device mounted
  while( !USBDevice.mounted() ) delay(1);
  Serial.println("joystick alltouch");
}

void loop()
{
  if ( !Joystick.ready() ) return;

  static uint16_t touchold;
  uint16_t touchnow = touch.touched();
  uint16_t changed = touchnow ^ touchold;

  if (USE_TOUCH) {
    if (changed) {
      uint16_t bitmask = 1;
      for (int i = 0; i < 12; i++) {
        if (changed & bitmask) {
          if (touchnow & bitmask) {
            switch (i) {
              case 8:
                Joystick.yAxis(-MAX_AXIS);
                break;
              case 9:
                Joystick.yAxis(MAX_AXIS);
                break;
              case 10:
                Joystick.xAxis(-MAX_AXIS);
                break;
              case 11:
                Joystick.xAxis(MAX_AXIS);
                break;
              default:
                Joystick.press(i);
                break;
            }
          }
          else {
            switch (i) {
              case 8:
                /* fall through */
              case 9:
                Joystick.yAxis(0);
                break;
              case 10:
                /* fall through */
              case 11:
                Joystick.xAxis(0);
                break;
              default:
                Joystick.release(i);
                break;
            }
          }
        }
        bitmask <<= 1;
      } // for each touch input
      touchold = touchnow;
    } // if at least one touch input changed
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].update();
    if ( buttons[i].fell() ) {
      Joystick.press(i);
    }
    else if ( buttons[i].rose() ) {
      Joystick.release(i);
    }
  }

  // Joystick functions above only set the values.
  // This writes the HID report to the host.
  Joystick.loop();
}
