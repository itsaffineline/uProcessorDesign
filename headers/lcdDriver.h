#ifndef LCDDRIVER_H
#define LCDDRIVER_H
/* all the required header for project here*/

#include "boarddefs.h"

/* LCD specific variables */
#define TFTWIDTH 240
#define TFTHEIGHT 320

/* A good way to stay consistent with writing signals */
#define ACTIVE	0
#define IDLE 	1
#define CMD 	0
#define DATA 	1

/* definition of colors in 2-bytes*/
#define BLACK 0x0000
#define GRAY 0xD6BA
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

/*ILI9341 Registers*/
#define ILI9341_SOFTRESET           0x01
#define ILI9341_SLEEPIN             0x10
#define ILI9341_SLEEPOUT            0x11
#define ILI9341_NORMALDISP          0x13
#define ILI9341_INVERTOFF           0x20
#define ILI9341_INVERTON            0x21
#define ILI9341_GAMMASET            0x26
#define ILI9341_DISPLAYOFF          0x28
#define ILI9341_DISPLAYON           0x29
#define ILI9341_COLADDRSET          0x2A
#define ILI9341_PAGEADDRSET         0x2B
#define ILI9341_MEMORYWRITE         0x2C
#define ILI9341_PIXELFORMAT         0x3A
#define ILI9341_FRAMECONTROL        0xB1
#define ILI9341_DISPLAYFUNC         0xB6
#define ILI9341_ENTRYMODE           0xB7
#define ILI9341_POWERCONTROL1       0xC0
#define ILI9341_POWERCONTROL2       0xC1
#define ILI9341_VCOMCONTROL1        0xC5
#define ILI9341_VCOMCONTROL2        0xC7
#define ILI9341_MEMCONTROL          0x36
#define ILI9341_MADCTL              0x36

#define ILI9341_MADCTL_MY  	   0x80
#define ILI9341_MADCTL_MX  	   0x40
#define ILI9341_MADCTL_MV  	   0x20
#define ILI9341_MADCTL_ML  	   0x10
#define ILI9341_MADCTL_RGB 	   0x00
#define ILI9341_MADCTL_BGR 	   0x08
#define ILI9341_MADCTL_MH  	   0x04


//Prototypes!

void delay (int16_t d);

inline void lcdWrite8Reg(uint8_t d);

inline void lcdWrite8Data(uint8_t d);

inline void lcdWrite8DataBlocking(uint8_t d);

inline void lcdWriteRegData8(uint8_t a, uint8_t d);

void lcdWriteRegData16(uint16_t a, uint16_t d);

// Cursor functions
void setCursor(uint16_t x, uint16_t y);

void setCursorX(uint16_t x);

void setCursorY(uint16_t y);

uint16_t getCursorX(void);

uint16_t getCursorY(void);

// Text functions
void setTextSize(uint8_t s);

uint8_t getTextSize(void);

void printChar(char c);

void print(char *str);

void println(char *str);

void printByteDecimal(uint8_t d);

void printNibbleHexadecimal(uint8_t d);

void printByteHexadecimal(uint8_t d);

// Graphic functions
void setForegroundColor(uint16_t color);

void setBackgroundColor(uint16_t color);

void drawPixel(uint16_t x3,uint16_t y3,uint16_t color1);

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void drawChar(uint8_t c);

void testCircles(uint8_t radius, uint16_t color);

void fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color);

void fillScreen(void);

// Utility functions
void setRotation(uint8_t flag);

void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void tftLcdInit(void);

uint16_t reverse(uint8_t d);

void writeSomeLines(void);

#endif

