#ifndef MAX_STATS_SCREEN_H
#define MAX_STATS_SCREEN_H

#if defined(TEENSYDUINO)
  #include <ST7735_t3.h>
  #include <ST7789_t3.h>
#else
  #include <Adafruit_ST7735.h>
  #include <Adafruit_ST7789.h>
#endif

#include "config.h"

// Screen states, for what to display, when to display it, and how!
typedef enum
{
  Unknown,
  WaitingConnection,
  ShowStats,
} ScreenState;

void screen_begin();

void screen_loop();

#endif
