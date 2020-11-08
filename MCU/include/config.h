/***********************************\
 * MaxStats - NotQuiteApex         *
 * CONFIG - edit values in here to *
 * configure your experience.      *
\***********************************/

// Screen types. Change depending on what screen you're using!
// SCR_7735, SCR_7789
#define SCR_TYPE SCR_7789

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

// Uncomment to use the Keyboard feature.
#define ENABLE_KEYBOARD

#ifdef ENABLE_KEYBOARD
  // Keyboard configuration
  #define KEY_COUNT 8

  // Pin to button input
  #define KEY_PIN0 22
  #define KEY_PIN1 21
  #define KEY_PIN2 20
  #define KEY_PIN3 19
  #define KEY_PIN4 18
  #define KEY_PIN5 17
  #define KEY_PIN6 16
  #define KEY_PIN7 15

  // Keyboard key to bind the button to
  #define KEY_BIND0 KEY_F13
  #define KEY_BIND1 KEY_F14
  #define KEY_BIND2 KEY_F15
  #define KEY_BIND3 KEY_F16
  #define KEY_BIND4 KEY_F17
  #define KEY_BIND5 KEY_F18
  #define KEY_BIND6 KEY_F19
  #define KEY_BIND7 KEY_F10
#endif
