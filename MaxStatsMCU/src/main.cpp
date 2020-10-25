/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

// Standard include, because we use Arduino in the household.
#include <Arduino.h>

// Include config and macro stuffs
#include "macros.h"
#include "config.h"

#ifndef SCR_TYPE
  #error SCRTYPE must be defined in `config.h`!
#elif SCR_TYPE == SCR_7735
  #include <Adafruit_ST7735.h>
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#elif SCR_TYPE == SCR_7789
  #include <Adafruit_ST7789.h>
  Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
#else
  #error SCR_TYPE must be defined properly in `config.h`!
#endif

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
// Data that we receive only at the time a connection is made.
String cpuName = "";
String gpuName = "";
String ramCount = "";
// Data we constantly receive after a connection is made.

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

  // Set up the TFT
  delay(200);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(SCR_ROTATE);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
}

void loop() {
  tft.fillScreen(ST7735_BLACK);
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
      // wait for info on CPU, GPU, and RAM
      if (receive_once_data()) {
        commstagepart = 1;
      }
    } else if (commstagepart == 1) {
      // respond that we got it, and move to the next stage
      Serial.write("222");
      commstage = SerialStage::ContinuousStats;
      commstagepart = 0;
    }
  }
  else if (commstage == SerialStage::ContinuousStats)
  {
    // Format string of continuous stats
    // $"{cpuFreq}|{cpuTemp}|{cpuLoad}|{ramUsed}|{gpuTemp}|" +
    // $"{gpuCoreClock}|{gpuCoreLoad}|{gpuVramClock}|{gpuVramLoad}|"

    if (commstagepart == 0) {
      // recieve
    } else if (commstagepart == 1) {
      // respond with "333"
    }
  }

  tft.setCursor(0, 00); tft.print(cpuName);
  tft.setCursor(0, 10); tft.print(gpuName);
  tft.setCursor(0, 20); tft.print(ramCount);

  delay(1000);
}
