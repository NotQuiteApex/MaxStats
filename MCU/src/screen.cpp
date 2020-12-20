#include <Arduino.h>

#include "screen.h"

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

ScreenState screenstate, previousstate;

// Data we store!
extern String cpuName;
extern String gpuName;
extern String ramCount;
extern String cpuFreq;
extern String cpuTemp;
extern String cpuLoad;
extern String ramUsed;
extern String gpuTemp;
extern String gpuCoreClock;
extern String gpuCoreLoad;
extern String gpuVramClock;
extern String gpuVramLoad;

// Async counter
extern u32 countermax;

void screen_begin() {
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
}

void screen_loop() {
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
