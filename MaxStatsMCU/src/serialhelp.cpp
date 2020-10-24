#include <Arduino.h>

#include "serialhelp.h"

extern String inputString;

void serial_recieve() {
  inputString = "";
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      // Read in data to a string
      int readbyte = Serial.read();
      if (readbyte != -1)
        inputString += (char)readbyte;
    }
  }
}

bool serial_matches(char * matcher) {
  serial_recieve();

  return inputString.equals(matcher);
}