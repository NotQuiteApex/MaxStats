/***********************************\
 * MaxStats - NotQuiteApex         *
 * CONFIG - edit values in here to *
 * configure your experience.      *
\***********************************/

// IGNORE THESE! DON'T TOUCH THESE!
// Screen types
#define SCR_7735 7735
#define SCR_7789 7789
// Tab types
#define TAB_GREEN INITR_GREENTAB 
#define TAB_BLACK INITR_BLACKTAB 
// Edit everything below here.

// Screen types. Change depending on what screen you're using!
#define SCR_TYPE SCR_7789 // SCR_7735, SCR_7789

// TFT settings
const u16 scr_width = 320;    // Screen width
const u16 scr_height = 240;   // Screen height
const u8 scr_rotate = 1;      // Screen orientation, 0=Portrait, 1=Landscape, 2=InvertPortrait, 3= InvertLandscape
const u8 scr_scale = 2;       // Scales all the text, coords, etc for the screen.
const u8 scr_tab = TAB_GREEN; // Tab type, for st7735 screens (not applicable for st7789)
// TFT pins
const u8 pin_cs  = 10; // Chip select
const u8 pin_dc  = 9;  // Data command
const u8 pin_rst = 8;  // Reset (set to -1 if not used)
const u8 pin_bl  = 7;  // Backlight

// Uncomment to use the Keyboard feature.
#define ENABLE_KEYBOARD

#ifdef ENABLE_KEYBOARD
  // Keyboard configuration
  // Pins to configure, and their associated keys
  const KeyConf keys[] = {
    {22, KEY_F13},
    {21, KEY_F14},
    {20, KEY_F15},
    {19, KEY_F16},
    {18, KEY_F17},
    {17, KEY_F18},
    {16, KEY_F19},
    {15, KEY_F20},
  };
  // pinMode for the keys.
  const u8 keyPinMode = INPUT_PULLUP;
  // Set to true if the keys are pulled up in any way
  // (through arduino or physical pull ups)
  const bool keyPulledUp = true;
#endif
