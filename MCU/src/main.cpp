/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

// Standard include, because we use Arduino in the household.
#include <Arduino.h>

// Necessary includes, compiler will optimize out what we don't use
#include <Bounce2.h>

#include "sh_types.h"

// Include config
#include "config.h"

// Helper files
#include "screen.h"
#include "serialhelp.h"

// Keyboard setup
#ifdef ENABLE_KEYBOARD
  const u8 keyCount = sizeof(keys) / sizeof(keys[0]);
  Bounce deboun[keyCount];
#endif


// error checker, if this goes up to a certain amount, reset everything.
byte errorcount = 0;

// Time counter, so we can run asynchronously.
u32 time = 0;
u32 prev = 0;
u32 delta = 0;

u32 counter = 0;
u32 countermax = 0;

void setup() {
  serial_begin();

  screen_begin();

  #ifdef ENABLE_KEYBOARD
    for (u8 i = 0; i < keyCount; i++) {
      deboun[i] = Bounce();
      deboun[i].attach(keys[i].pin, keyPinMode);
      deboun[i].interval(10);
    }
  #endif
}

void loop() {
  #ifdef ENABLE_KEYBOARD
    for (u8 i = 0; i < keyCount; i++) {
      deboun[i].update();
      
      if (keyPinMode == INPUT_PULLUP) {
        if (deboun[i].fell()) {
          Keyboard.press(keys[i].key);
        } else if (deboun[i].rose()) {
          Keyboard.release(keys[i].key);
        }
      } else {
        if (deboun[i].rose()) {
          Keyboard.press(keys[i].key);
        } else if (deboun[i].fell()) {
          Keyboard.release(keys[i].key);
        }

      }
    }
  #endif

  prev = time;
  time = millis();

  delta = time - prev;

  counter += delta;

  // Asynchronous updates to serial and stuff, so keyboard isn't interrupted.
  if (counter >= countermax) {
    countermax = 0;
    counter = 0;

    serial_loop();
    screen_loop();
  }
}
