#include <Arduino.h>

#include "serialhelp.h"

// Input string, we use it for storing all our data.
extern String inputString;

// Specific hardware details, we only need them once.
extern String cpuName = "";
extern String gpuName = "";
extern String ramCount = "";

void serial_receive() {
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
  serial_receive();

  return inputString.equals(matcher);
}

bool receive_once_data() {
  // string format of computer parts stats
  // $"{cpuName}|{gpuName}|{ramTotal}GB|"

  serial_receive();
  if (!inputString.equals("")) {
        // process the names!
        String temp = "";
        temp.reserve(32);
        unsigned int idx1 = 0;
        unsigned int idx2 = 0;
        byte count = 0;

        // Loop through the string, finding the pipe seperators
        for (unsigned int i = 0; i < inputString.length(); i++) {
          if (i == (inputString.length() - 1))
            continue;
          
          if (inputString[i] == '|') {
            idx1 = idx2;
            if (inputString[idx1] == '|')
              idx1++;
            idx2 = i;

            temp = inputString.substring(idx1, idx2);

            if (count == 0)
              cpuName = temp;
            else if (count == 1)
              gpuName = temp;
            else if (count == 2)
              ramCount = temp;

            count++;
          }
        }

        return true;
      }
      
  return false;
}

bool receive_cont_data() {
  serial_receive();
}