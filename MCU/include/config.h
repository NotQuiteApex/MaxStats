/***********************************\
 * MaxStats - NotQuiteApex         *
 * CONFIG - edit values in here to *
 * configure your experience.      *
\***********************************/

// Screen types. Change depending on what screen you're using!
// SCR_7735, SCR_7789
#define SCR_TYPE SCR_7735

// Rotate the screen.
// 0 = Portrait, 1 = Landscape,
// 2 = Upside-down portrait, 3 = Upside-down landscape
#define SCR_ROTATE 1

// Settings specific to certain screentypes, edit as necessary.
#if SCR_TYPE == SCR_7735
  #define SCR_SCALE  1
  #define TAB_TYPE   INITR_GREENTAB
  #define SCR_WIDTH  160
  #define SCR_HEIGHT 128
#elif SCR_TYPE == SCR_7789
  #define SCR_SCALE  2
  #define SCR_WIDTH  320
  #define SCR_HEIGHT 240
#endif

// TFT pins
#define PIN_CS   10	// Chip select
#define PIN_DC   9  // Data command
#define PIN_RST  8  // Reset (set to -1 if not in use)
#define PIN_BL   7  // Backlight
