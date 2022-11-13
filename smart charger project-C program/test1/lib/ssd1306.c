#include <avr/pgmspace.h>
#include <string.h>
#include "font.h"
#include "twi.h"
#include "ssd1306.h"

const uint8_t INIT_SSD1306[] PROGMEM = {

  18,

  0, SSD1306_DISPLAY_OFF,
  // 0xA8
  1, SSD1306_SET_MUX_RATIO, 0x3F,


  1, SSD1306_MEMORY_ADDR_MODE, 0x00,


  2, SSD1306_SET_COLUMN_ADDR, START_COLUMN_ADDR, END_COLUMN_ADDR,

  2, SSD1306_SET_PAGE_ADDR, START_PAGE_ADDR, END_PAGE_ADDR,

  0, SSD1306_SET_START_LINE,

  1, SSD1306_DISPLAY_OFFSET, 0x00,

  0, SSD1306_SEG_REMAP_OP,

  0, SSD1306_COM_SCAN_DIR_OP,

  1, SSD1306_COM_PIN_CONF, 0x12,

  1, SSD1306_SET_CONTRAST, 0x7F,

  0, SSD1306_DIS_ENT_DISP_ON,

  0, SSD1306_DIS_NORMAL,

  1, SSD1306_SET_OSC_FREQ, 0x80,

  1, SSD1306_SET_PRECHARGE, 0xc2,

  1, SSD1306_VCOM_DESELECT, 0x20,

  1, SSD1306_SET_CHAR_REG, 0x14,

  0, SSD1306_DISPLAY_ON
};


static char cacheMemLcd[CACHE_SIZE_MEM];


uint8_t SSD1306_Init (uint8_t address)
{ 

  const uint8_t *commands = INIT_SSD1306;

  unsigned short int no_of_commands = pgm_read_byte(commands++);

  uint8_t no_of_arguments;

  uint8_t command;

  uint8_t status = INIT_STATUS;

  TWI_Init ();


  status = SSD1306_Send_StartAndSLAW (address);

  if (SSD1306_SUCCESS != status) {
    // error
    return status;
  }


  while (no_of_commands) {


    no_of_arguments = pgm_read_byte (commands++);

    command = pgm_read_byte (commands++);


    status = SSD1306_Send_Command (command);

    if (SSD1306_SUCCESS != status) {

      return status;
    }


    while (no_of_arguments--) {

      status = SSD1306_Send_Command (pgm_read_byte(commands++));
      if (SSD1306_SUCCESS != status) {
        // error
        return status;
      }
    }

    no_of_commands--;
  }

  TWI_Stop ();

  return SSD1306_SUCCESS;
}


uint8_t SSD1306_Send_StartAndSLAW (uint8_t address)
{

  uint8_t status = INIT_STATUS;


  status = TWI_MT_Start ();

  if (SSD1306_SUCCESS != status) {

    return status;
  }


  status = TWI_MT_Send_SLAW (address);

  if (SSD1306_SUCCESS != status) {

    return status;
  }

  return SSD1306_SUCCESS;
}

uint8_t SSD1306_Send_Command (uint8_t command)
{

  uint8_t status = INIT_STATUS;


  status = TWI_MT_Send_Data (SSD1306_COMMAND);

  if (SSD1306_SUCCESS != status) {

    return status;
  }

  status = TWI_MT_Send_Data (command);

  if (SSD1306_SUCCESS != status) {

    return status;
  }


  return SSD1306_SUCCESS;
}


uint8_t SSD1306_NormalScreen (uint8_t address)
{

  uint8_t status = INIT_STATUS;


  status = SSD1306_Send_StartAndSLAW (address);

  if (SSD1306_SUCCESS != status) {

    return status;
  }

  
  status = SSD1306_Send_Command (SSD1306_DIS_NORMAL);

  if (SSD1306_SUCCESS != status) {

    return status;
  }

  return SSD1306_SUCCESS;
}


uint8_t SSD1306_InverseScreen (uint8_t address)
{

  uint8_t status = INIT_STATUS;


  status = SSD1306_Send_StartAndSLAW (address);

  if (SSD1306_SUCCESS != status) {

    return status;
  }
  
  status = SSD1306_Send_Command (SSD1306_DIS_INVERSE);

  if (SSD1306_SUCCESS != status) {

    return status;
  }


  return SSD1306_SUCCESS;
}


uint8_t SSD1306_UpdateScreen (uint8_t address)
{

  uint8_t status = INIT_STATUS;

  uint16_t i = 0;


  status = SSD1306_Send_StartAndSLAW (address);
 
  if (SSD1306_SUCCESS != status) {
    return status;
  }
  
  status = TWI_MT_Send_Data (SSD1306_DATA_STREAM);
  if (SSD1306_SUCCESS != status) {
    // error
    return status;
  }


  while (i < CACHE_SIZE_MEM) {

    status = TWI_MT_Send_Data (cacheMemLcd[i]);

    if (SSD1306_SUCCESS != status) {

      return status;
    }

    i++;
  }


  TWI_Stop ();


  return SSD1306_SUCCESS;
}


void SSD1306_ClearScreen (void)
{

  memset (cacheMemLcd, 0x00, CACHE_SIZE_MEM);
}


void SSD1306_SetPosition (uint8_t x, uint8_t y) 
{

  _counter = x + (y << 7);
}


uint8_t SSD1306_UpdTxtPosition (void) 
{

  uint8_t y = _counter >> 7;

  uint8_t x = _counter - (y << 7);

  uint8_t x_new = x + CHARS_COLS_LENGTH + 1;


  if (x_new > END_COLUMN_ADDR) {

    if (y > END_PAGE_ADDR) {

      return SSD1306_ERROR;

    } else if (y < (END_PAGE_ADDR-1)) {

      _counter = ((++y) << 7);
    }
  }

  return SSD1306_SUCCESS;
}


uint8_t SSD1306_DrawChar (char character)
{

  uint8_t i = 0;

  if (SSD1306_UpdTxtPosition () == SSD1306_ERROR) {

    return SSD1306_ERROR;
  }

  while (i < CHARS_COLS_LENGTH) {

    cacheMemLcd[_counter++] = pgm_read_byte(&FONTS[character-32][i++]);
  }


  _counter++;


  return SSD1306_SUCCESS;
}


void SSD1306_DrawString (char *str)
{

  int i = 0;

  while (str[i] != '\0') {

    SSD1306_DrawChar (str[i++]);
  }
}


uint8_t SSD1306_DrawPixel (uint8_t x, uint8_t y)
{
  uint8_t page = 0;
  uint8_t pixel = 0;

  if ((x > MAX_X) && (y > MAX_Y)) {

    return SSD1306_ERROR;
  }

  page = y >> 3;

  pixel = 1 << (y - (page << 3));

  _counter = x + (page << 7);

  cacheMemLcd[_counter++] |= pixel;


  return SSD1306_SUCCESS;
}


uint8_t SSD1306_DrawLine (uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{

  int16_t D;

  int16_t delta_x, delta_y;

  int16_t trace_x = 1, trace_y = 1;


  delta_x = x2 - x1;

  delta_y = y2 - y1;


  if (delta_x < 0) {

    delta_x = -delta_x;

    trace_x = -trace_x;
  }


  if (delta_y < 0) {

    delta_y = -delta_y;

    trace_y = -trace_y;
  }


  if (delta_y < delta_x) {

    D = (delta_y << 1) - delta_x;

    SSD1306_DrawPixel (x1, y1);

    while (x1 != x2) {

      x1 += trace_x;

      if (D >= 0) {

        y1 += trace_y;

        D -= 2*delta_x;    
      }

      D += 2*delta_y;

      SSD1306_DrawPixel (x1, y1);
    }

  } else {

    D = delta_y - (delta_x << 1);

    SSD1306_DrawPixel (x1, y1);

    while (y1 != y2) {

      y1 += trace_y;

      if (D <= 0) {

        x1 += trace_x;

        D += 2*delta_y;    
      }

      D -= 2*delta_x;

      SSD1306_DrawPixel (x1, y1);
    }
  }

  return SSD1306_SUCCESS;
}
