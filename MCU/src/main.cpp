/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

// Standard include, because we use Arduino in the household.
#include <Arduino.h>

// Necessary includes, compiler will optimize out what we don't use
#if defined(TEENSYDUINO)
  #include <ST7735_t3.h>
  #include <ST7789_t3.h>
#else
  #include <Adafruit_ST7735.h>
  #include <Adafruit_ST7789.h>
#endif
#include <Keyboard.h>
#include <Bounce2.h>

#include "sh_types.h"

struct KeyConf {
    u8 pin;
    u32 key;
    KeyConf(u8 pin, u32 key) : pin(pin), key(key) {};
};


// Include config
#include "config.h"


// Screen determining
#ifndef SCR_TYPE
  #error SCRTYPE must be defined in `config.h`!
#elif SCR_TYPE == SCR_7735
  #if defined(TEENSYDUINO)
    ST7735_t3 tft = ST7735_t3(pin_cs, pin_dc, 12, 13, pin_rst);
  #else
    Adafruit_ST7735 tft = Adafruit_ST7735(pin_cs, pin_dc, pin_rst);
  #endif
#elif SCR_TYPE == SCR_7789
  #if defined(TEENSYDUINO)
    ST7789_t3 tft = ST7789_t3(pin_cs, pin_dc, 12, 13, pin_rst);
  #else
    Adafruit_ST7789 tft = Adafruit_ST7789(pin_cs, pin_dc, pin_rst);
  #endif
#else
  #error SCR_TYPE must be defined properly in `config.h`!
#endif

// Keyboard setup
#ifdef ENABLE_KEYBOARD
  const u8 keyCount = sizeof(keys) / sizeof(keys[0]);
  Bounce deboun[keyCount];
#endif


// Serial helper functions file
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

// Time counter, so we can run asynchronously.
unsigned long time = 0;
unsigned long prev = 0;
unsigned long delta = 0;

unsigned long counter = 0;
unsigned long countermax = 0;

void setup()
{
  // Set up serial communication for the board
  #if defined (TEENSYDUINO)
    Serial.begin(115200); // Teensy doesn't have settings for Serial.
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
    tft.initR(scr_tab);
  #elif SCR_TYPE == SCR_7789
    tft.init(scr_width, scr_height);
  #endif
  delay(100);
  tft.setRotation(scr_rotate);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  
  // Set up backlight, so we can adjust it later
  pinMode(pin_bl, OUTPUT);
  digitalWrite(pin_bl, HIGH);

  previousstate = ScreenState::Unknown;
  screenstate = ScreenState::WaitingConnection;

  #ifdef ENABLE_KEYBOARD
    for (u8 i = 0; i < keyCount; i++) {
      deboun[i] = Bounce();
      deboun[i].attach(keys[i].pin, keyPinMode);
      deboun[i].interval(10);
    }
  #endif
}

void loop()
{
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
          countermax = 1000; //delay(1000);
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
          countermax = 1000; //delay(1000);
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
        tft.setTextSize(scr_scale);
        
        tft.setCursor(55 * scr_scale, 50 * scr_scale);
        tft.print("MaxStats");
        tft.setCursor(5 * scr_scale, 60 * scr_scale);
        tft.print("Waiting for connection...");
      }
      else
      {
        tft.setTextSize(1 * scr_scale);

        tft.setCursor(4 * scr_scale, 3 * scr_scale);
        if (cpuName.substring(0, 3).equals("AMD"))
          tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
        else
          tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
        tft.print(cpuName);

        tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft.setCursor(0 * scr_scale, 28 * scr_scale); tft.print("Freq GHz");
        tft.setCursor(68 * scr_scale, 28 * scr_scale); tft.print("Load %");
        tft.setCursor(112 * scr_scale, 28 * scr_scale); tft.print("Temp \xF7 C");

        // dividers
        tft.drawLine(51 * scr_scale, 12 * scr_scale, 51 * scr_scale, 34 * scr_scale, ST77XX_WHITE);
        tft.drawLine(106 * scr_scale, 12 * scr_scale, 106 * scr_scale, 34 * scr_scale, ST77XX_WHITE);

        if (gpuName.substring(0, 3).equals("AMD"))
          tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
        else
          tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
        tft.setCursor(4 * scr_scale, 47 * scr_scale); tft.print(gpuName);

        tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft.setCursor(12 * scr_scale, 72 * scr_scale); tft.print("Load %");
        tft.setCursor(68 * scr_scale, 72 * scr_scale); tft.print("VRAM %");

        tft.setCursor(0 * scr_scale, 96 * scr_scale); tft.print("Core MHz");
        tft.setCursor(56 * scr_scale, 96 * scr_scale); tft.print("VRAM MHz");
        tft.setCursor(112 * scr_scale, 72 * scr_scale); tft.print("Temp \xF7 C");
        
        // dividers
        tft.drawLine(51 * scr_scale, 56 * scr_scale, 51 * scr_scale, 104 * scr_scale, ST77XX_WHITE);
        tft.drawLine(106 * scr_scale, 56 * scr_scale, 106 * scr_scale, 104 * scr_scale, ST77XX_WHITE);

        tft.setCursor(scr_width - 6 * 10 * scr_scale, scr_height - 8 * scr_scale); tft.print("RAM: " + ramCount);
      }
    }

    // Repeat draws!
    if (screenstate == ScreenState::ShowStats)
    {
      tft.setTextSize(2 * scr_scale);

      tft.setCursor(0 * scr_scale, 12 * scr_scale); tft.print(cpuFreq);
      tft.setCursor(56 * scr_scale, 12 * scr_scale); tft.print(cpuLoad);
      tft.setCursor(112 * scr_scale, 12 * scr_scale); tft.print(cpuTemp);

      tft.setCursor(0 * scr_scale, 56 * scr_scale); tft.print(gpuCoreLoad);
      tft.setCursor(56 * scr_scale, 56 * scr_scale); tft.print(gpuVramLoad);
      tft.setCursor(112 * scr_scale, 56 * scr_scale); tft.print(gpuTemp);

      tft.setTextSize(1 * scr_scale);
      tft.setCursor(12 * scr_scale, 88 * scr_scale); tft.print(gpuCoreClock);
      tft.setCursor(68 * scr_scale, 88 * scr_scale); tft.print(gpuVramClock);

      tft.setTextSize(2 * scr_scale);
      tft.setCursor(scr_width - 5 * 10 * scr_scale, scr_height - 24 * scr_scale); tft.print(ramUsed);

      countermax = 1000;
    }
  }
}
