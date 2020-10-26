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
enum SerialStage
{
  Handshake,      // The initial stage, where the computer and MCU greet
  ComputerParts,  // The computer sends some constant data over the wire
  ContinuousStats // The computer sends data that gets updated over time
} commstage;

// The parts of comm stages (stage recieve and response)
int commstagepart;

// The data string we use to store received data
String inputString = "";

// A temporary string for sorting data in the serial files.
String tempInput = "";

// Data we store!
// Data that we receive only at the time a connection is made.
String cpuName = "-";
String gpuName = "-";
String ramCount = "-";
// Data we constantly receive after a connection is made.
String cpuFreq = "-";
String cpuTemp = "-";
String cpuLoad = "-";
String ramUsed = "-";
String gpuTemp = "-";
String gpuCoreClock = "-";
String gpuCoreLoad = "-";
String gpuVramClock = "-";
String gpuVramLoad = "-";

// error checker, if this goes up to a certain amount, reset everything.
byte errorcount = 0;

void setup()
{
  // Set up serial communication for the board
  #if defined (TEENSYDUINO)
  Serial.begin(115200);
  #else
  Serial.begin(115200, SERIAL_8E2);
  #endif

  // Initialize the communications stage
  commstage = SerialStage::Handshake;
  commstagepart = 0;

  // Reserve some space for the strings
  inputString.reserve(125);
  tempInput.reserve(50);

  cpuName.reserve(30);
  gpuName.reserve(40);
  ramCount.reserve(4);

  cpuFreq.reserve(4);
  cpuTemp.reserve(4);
  cpuLoad.reserve(4);
  ramUsed.reserve(4);
  gpuTemp.reserve(4);
  gpuCoreClock.reserve(8);
  gpuCoreLoad.reserve(4);
  gpuVramClock.reserve(8);
  gpuVramLoad.reserve(6);

  // Set up the TFT
  //delay(100);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(SCR_ROTATE);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
}

void loop()
{
  bool shouldwait = true;

  tft.fillScreen(ST77XX_BLACK);

  // we mustve lost comms, reset!
  if (errorcount >= 5)
  {
    errorcount = 0;

    commstage = SerialStage::Handshake;
    commstagepart = 0;

    cpuName = "-";
    gpuName = "-";
    ramCount = "-";

    cpuFreq = "-";
    cpuTemp = "-";
    cpuLoad = "-";
    ramUsed = "-";
    gpuTemp = "-";
    gpuCoreClock = "-";
    gpuCoreLoad = "-";
    gpuVramClock = "-";
    gpuVramLoad = "-";
  }

  if (commstage == SerialStage::Handshake)
  {
    if (commstagepart == 0)
    {
      if (serial_matches("010"))
      {
        commstagepart = 1;
        shouldwait = false;
      }
    }
    else if (commstagepart == 1)
    {
      Serial.write("101");
      commstage = SerialStage::ComputerParts;
      commstagepart = 0;
      shouldwait = false;
    }
  }
  else if (commstage == SerialStage::ComputerParts)
  {
    if (commstagepart == 0)
    {
      // wait for info on CPU, GPU, and RAM
      if (receive_once_data())
      {
        commstagepart = 1;
        errorcount = 0;
        shouldwait = false;
      }
      else
        errorcount++;
    }
    else if (commstagepart == 1)
    {
      // respond that we got it, and move to the next stage
      Serial.write("222");
      commstage = SerialStage::ContinuousStats;
      commstagepart = 0;
      shouldwait = false;
    }
  }
  else if (commstage == SerialStage::ContinuousStats)
  {
    if (commstagepart == 0)
    {
      // recieve
      if (receive_cont_data())
      {
        commstagepart = 1;
        errorcount = 0;
      }
      else
        errorcount++;
    }
    else if (commstagepart == 1)
    {
      // respond that we got it, and repeat
      Serial.write("333");
      commstagepart = 0;
      shouldwait = false;
    }
  }

  if (shouldwait)
  {
    tft.setCursor(0, 00); tft.print("CPU: " + cpuName);
    tft.setCursor(0, 10); tft.print("Freq: " + cpuFreq + " GHz");
    tft.setCursor(0, 20); tft.print("Load: " + cpuLoad + " %");
    tft.setCursor(0, 30); tft.print("Temp: " + cpuTemp + " \xF7 C");

    tft.setCursor(0, 50); tft.print("GPU: " + gpuName);
    tft.setCursor(0, 60); tft.print("Temp: " + gpuTemp + " \xF7 C");
    tft.setCursor(0, 70); tft.print("Core Clock: " + gpuCoreClock + " MHz");
    tft.setCursor(0, 80); tft.print("Core Load: " + gpuCoreLoad + " %");
    tft.setCursor(0, 90); tft.print("VRAM Clock: " + gpuVramClock + " MHz");
    tft.setCursor(0, 100); tft.print("VRAM Load: " + gpuVramLoad + " %");

    tft.setCursor(0, 120); tft.print("RAM: " + ramUsed + " / " + ramCount);

    delay(1000);
  }
}
