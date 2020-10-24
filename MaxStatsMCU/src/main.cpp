/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

#include <Arduino.h>

#include "config.h"

#include <Adafruit_ST7735.h>
//#include <Adafruit_ST7789.h>
//#include <Adafruit_ST77xx.h>

#include "serialhelp.h"

// Stages of serial communication
enum SerialStage {
  Handshake,      // The initial stage, where the computer and MCU greet
  ComputerParts,  // The computer sends some constant data over the wire
  ContinuousStats // The computer sends data that gets updated over time
} commstage;

// The parts of comm stages (stage recieve and response)
int commstagepart;

// The data string we use to store received data
String inputString = "";

// Data we store!
String cpuName = "";
String gpuName = "";
String ramCount = "";

void setup() {
  // Set up serial communication for the board
  #if defined (TEENSYDUINO)
  Serial.begin(115200);
  #else
  Serial.begin(115200, SERIAL_8E2);
  #endif

  // Initialize the communications stage
  commstage = SerialStage::Handshake;
  commstagepart = 0;

  // Reserve some space for the string
  inputString.reserve(200);
}

void loop() {
  if (commstage == SerialStage::Handshake)
  {
    if (commstagepart == 0) {
      if (serial_matches("010")) {
        commstagepart = 1;
      }
    } else if (commstagepart == 1) {
      Serial.write("101");
      commstage = SerialStage::ComputerParts;
      commstagepart = 0;
    }
  }
  else if (commstage == SerialStage::ComputerParts)
  {
    if (commstagepart == 0) {
      // wait for info on cpu, gpu, and ram
      serial_recieve();
      if (!inputString.equals("")) {
        // process the names!
        String temp = "";
        temp.reserve(32);
        unsigned int idx1 = 0;
        unsigned int idx2 = 0;
        byte count = 0;

        // Loop through the string, finding the pipe seperators
        for (int i = 0; i < inputString.length(); i++) {
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

        commstagepart = 1;
      }
    } else if (commstagepart == 1) {

    }
  }
  else if (commstage == SerialStage::ContinuousStats)
  {

  }
}
