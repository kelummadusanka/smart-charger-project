#include <avr/pgmspace.h>

#ifndef __FONT_H__
#define __FONT_H__

  // Characters definition
  // -----------------------------------
  // number of columns for chars
  #define CHARS_COLS_LENGTH  5
  // @const Characters
  extern const uint8_t FONTS[][CHARS_COLS_LENGTH];

#endif
