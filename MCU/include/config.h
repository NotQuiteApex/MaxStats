/***********************************\
 * MaxStats - NotQuiteApex         *
 * CONFIG - edit values in here to *
 * configure your experience.      *
\***********************************/
#ifndef MAX_STATS_CONFIG_H
#define MAX_STATS_CONFIG_H

#include "config_setup.h"

// Screen types. Change depending on what screen you're using!
#define SCR_TYPE SCR_7789 // SCR_7735, SCR_7789

// TFT settings
const u16 scr_width = 240;    // Screen width
const u16 scr_height = 320;   // Screen height
const u8 scr_rotate = 1;      // Orientation, 0=Portrait, 1=Landscape, 2=InvPortrait, 3=InvLandscape
const u8 scr_scale = 2;       // Scales all the text, coords, etc for the screen.
const u8 scr_tab = TAB_GREEN; // Tab type, GREEN or BLACK, for st7735 screens (not applicable for st7789)
// TFT pins
const u8 pin_cs  = 10; // Chip select
const u8 pin_dc  = 9;  // Data command
const u8 pin_rst = 7;  // Reset (set to -1 if not used)
const u8 pin_bl  = 8;  // Backlight

// Uncomment to use the Keyboard feature.
#define ENABLE_KEYBOARD

#ifdef ENABLE_KEYBOARD
  // Keyboard configuration
  // Pins to configure, and their associated keys
  const KeyConf keys[] = {
    {2, KEY_F13},
    {3, KEY_F14},
    {4, KEY_F15},
    {A2, KEY_F16},
    {A1, KEY_F17},
    {A0, KEY_F18},
  };
  // pinMode for the keys.
  const u8 keyPinMode = INPUT_PULLUP;
  // Set to true if the keys are pulled up in any way
  // (through Arduino or physical pull ups)
  const bool keyPulledUp = true;
#endif

#endif
