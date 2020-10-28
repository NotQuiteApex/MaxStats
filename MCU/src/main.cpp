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
  Adafruit_ST7735 tft = Adafruit_ST7735(PIN_CS, PIN_DC, PIN_RST);
#elif SCR_TYPE == SCR_7789
  #include <Adafruit_ST7789.h>
  Adafruit_ST7789 tft = Adafruit_ST7789(PIN_CS, PIN_DC, PIN_RST);
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

// Screen states, for what to display, when to display it, and how!
enum ScreenState
{
  Unknown,
  WaitingConnection,
  ShowStats,
} screenstate, previousstate;

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
  #if SCR_TYPE == SCR_7735
    tft.initR(INITR_GREENTAB);
  #elif SCR_TYPE == SCR_7789
    tft.init(SCR_WIDTH, SCR_HEIGHT);
  #endif
  delay(100);
  tft.setRotation(SCR_ROTATE);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  
  pinMode(PIN_BL, OUTPUT);
  digitalWrite(PIN_BL, HIGH);

  previousstate = ScreenState::Unknown;
  screenstate = ScreenState::WaitingConnection;
}

void loop()
{
  // we mustve lost comms, reset!
  if (errorcount >= 5)
  {
    errorcount = 0;

    commstage = SerialStage::Handshake;
    commstagepart = 0;

    screenstate = ScreenState::WaitingConnection;

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
      }
    }
    else if (commstagepart == 1)
    {
      Serial.write("101");
      commstage = SerialStage::ComputerParts;
      commstagepart = 0;
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
      }
      else
      {
        errorcount++;
        delay(1000);
      }
    }
    else if (commstagepart == 1)
    {
      // respond that we got it, and move to the next stage
      Serial.write("222");
      commstage = SerialStage::ContinuousStats;
      screenstate = ScreenState::ShowStats;
      commstagepart = 0;
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
      {
        errorcount++;
        delay(1000);
      }
    }
    else if (commstagepart == 1)
    {
      // respond that we got it, and repeat
      Serial.write("333");
      commstagepart = 0;
    }
  }

  // Drawing on screen!
  if (screenstate != previousstate)
  {
    previousstate = screenstate;
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);

    // Initialize the new screen
    if (screenstate == ScreenState::WaitingConnection)
    {
      tft.setCursor(55 * SCR_SCALE, 50 * SCR_SCALE);
      tft.print("MaxStats");
      tft.setCursor(5 * SCR_SCALE, 60 * SCR_SCALE);
      tft.print("Waiting for connection...");
    }
    else
    {
      tft.setTextSize(1 * SCR_SCALE);

      tft.setCursor(4 * SCR_SCALE, 3 * SCR_SCALE);
      if (cpuName.substring(0, 3).equals("AMD"))
        tft.setTextColor(tft.color565(119, 185, 0), ST77XX_BLACK);
      else
        tft.setTextColor(tft.color565(18, 124, 193), ST77XX_BLACK);
      tft.print(cpuName);

      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.setCursor(0 * SCR_SCALE, 28 * SCR_SCALE); tft.print("Freq GHz");
      tft.setCursor(68 * SCR_SCALE, 28 * SCR_SCALE); tft.print("Load %");
      tft.setCursor(112 * SCR_SCALE, 28 * SCR_SCALE); tft.print("Temp \xF7 C");

      // dividers
      tft.drawLine(51 * SCR_SCALE, 12 * SCR_SCALE, 51 * SCR_SCALE, 34 * SCR_SCALE, ST77XX_WHITE);
      tft.drawLine(106 * SCR_SCALE, 12 * SCR_SCALE, 106 * SCR_SCALE, 34 * SCR_SCALE, ST77XX_WHITE);

      if (gpuName.substring(0, 3).equals("AMD"))
        tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
      else
        tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
      tft.setCursor(4 * SCR_SCALE, 47 * SCR_SCALE); tft.print(gpuName);

      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.setCursor(12 * SCR_SCALE, 72 * SCR_SCALE); tft.print("Load %");
      tft.setCursor(68 * SCR_SCALE, 72 * SCR_SCALE); tft.print("VRAM %");

      tft.setCursor(0 * SCR_SCALE, 96 * SCR_SCALE); tft.print("Load MHz");
      tft.setCursor(56 * SCR_SCALE, 96 * SCR_SCALE); tft.print("VRAM MHz");
      //tft.setCursor(0, 68); tft.print("CoreClk\n" + gpuCoreClock + "MHz");
      //tft.setCursor(48, 68); tft.print("VRAMClk\n " + gpuVramClock + "MHz");
      tft.setCursor(112 * SCR_SCALE, 72 * SCR_SCALE); tft.print("Temp \xF7 C");
      
      // dividers
      tft.drawLine(51 * SCR_SCALE, 56 * SCR_SCALE, 51 * SCR_SCALE, 104 * SCR_SCALE, ST77XX_WHITE);
      tft.drawLine(106 * SCR_SCALE, 56 * SCR_SCALE, 106 * SCR_SCALE, 104 * SCR_SCALE, ST77XX_WHITE);

      tft.setCursor(320 - 6 * 10 * SCR_SCALE, 120 * SCR_SCALE - 8 * SCR_SCALE); tft.print("RAM: " + ramCount);
    }
  }

  // Repeat draws!
  if (screenstate == ScreenState::ShowStats)
  {
    tft.setTextSize(2 * SCR_SCALE);

    tft.setCursor(0 * SCR_SCALE, 12 * SCR_SCALE); tft.print(cpuFreq);
    tft.setCursor(56 * SCR_SCALE, 12 * SCR_SCALE); tft.print(cpuLoad);
    tft.setCursor(112 * SCR_SCALE, 12 * SCR_SCALE); tft.print(cpuTemp);

    tft.setCursor(0 * SCR_SCALE, 56 * SCR_SCALE); tft.print(gpuCoreLoad);
    tft.setCursor(56 * SCR_SCALE, 56 * SCR_SCALE); tft.print(gpuVramLoad);
    tft.setCursor(112 * SCR_SCALE, 56 * SCR_SCALE); tft.print(gpuTemp);

    tft.setTextSize(1 * SCR_SCALE);
    tft.setCursor(12 * SCR_SCALE, 88 * SCR_SCALE); tft.print(gpuCoreClock);
    tft.setCursor(68 * SCR_SCALE, 88 * SCR_SCALE); tft.print(gpuVramClock);

    tft.setTextSize(2 * SCR_SCALE);
    tft.setCursor(320 - 5 * 10 * SCR_SCALE, 120 * SCR_SCALE - 24 * SCR_SCALE); tft.print(ramUsed);
  }
}
