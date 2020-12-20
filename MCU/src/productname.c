#if defined(TEENSYDUINO)

// https://forum.pjrc.com/threads/23796-How-to-change-Teensy-3-0-PRODUCT_NAME

#include <usb_names.h>

#define MANUFACTURER_NAME     {'F','r','i','e','n','d','T','e','a','m','I','n','c','.'}
#define MANUFACTURER_NAME_LEN 14
#define PRODUCT_NAME          {'M','a','x','S','t','a','t','s'}
#define PRODUCT_NAME_LEN      8

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
  2 + MANUFACTURER_NAME_LEN * 2,
  3,
  MANUFACTURER_NAME
};
struct usb_string_descriptor_struct usb_string_product_name = {
  2 + PRODUCT_NAME_LEN * 2,
  3,
  PRODUCT_NAME
};
struct usb_string_descriptor_struct usb_string_serial_number = {
  12,
  3,
  {
    0,0,7,0,0,0,7,0,0,7  }
};

#endif