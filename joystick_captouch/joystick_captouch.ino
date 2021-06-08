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
#include <Bounce2.h>
#include "captouch_joystick.h"
#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 touch = Adafruit_MPR121();

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {4, 5, 6, 7, 8, 9, 10, 11};

Bounce * buttons = new Bounce[NUM_BUTTONS];

const unsigned int PI_PICO_ADC_BITS = 12;
const unsigned int MAX_ADC_VALUE = (1 << PI_PICO_ADC_BITS) - 1;

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
  Serial.println("joystick captouch");
}

void loop()
{
  if ( !Joystick.ready() ) return;

  if (USE_TOUCH) {
    static uint16_t touchold;
    uint16_t touchnow = touch.touched();
    uint16_t changed = touchnow ^ touchold;

    if (changed) {
      uint16_t bitmask = 1;
      for (int i = 0; i < 12; i++) {
        if (changed & bitmask) {
          if (touchnow & bitmask) {
            Joystick.press(i);
          }
          else {
            Joystick.release(i);
          }
        }
        bitmask <<= 1;
      }
      touchold = touchnow;
    }
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

  // A0 = x axis
  // A1 = y axis
  Joystick.xAxis(map(analogRead(A0), 0, MAX_ADC_VALUE, -32767, 32767));
  Joystick.yAxis(map(analogRead(A1), 0, MAX_ADC_VALUE, 32767, -32767));

  // Joystick functions above only set the values.
  // This writes the HID report to the host.
  Joystick.loop();
}
