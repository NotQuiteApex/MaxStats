#ifndef MAX_STATS_CONFIG_SETUP_H
#define MAX_STATS_CONFIG_SETUP_H

// DON'T TOUCH THESE! For config.h.
#include <Keyboard.h>
#include "sh_types.h"

#if defined(TEENSYDUINO)
  #include <ST7735_t3.h>
  #include <ST7789_t3.h>
#else
  #include <Adafruit_ST7735.h>
  #include <Adafruit_ST7789.h>
#endif

// Screen types
#define SCR_7735 7735
#define SCR_7789 7789
// Tab types
#define TAB_GREEN INITR_GREENTAB 
#define TAB_BLACK INITR_BLACKTAB 

#endif