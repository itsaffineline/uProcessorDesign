#ifndef BOARDDEFS_H
#define BOARDDEFS_H

#include "at89x51.h"
#include <stdint.h>

// Keypad definitions
#define KEYPAD_PORT P1

// IO addresses
#define RTC_ADDRESS     0x0000
#define LCD_ADDRESS     0x4000
#define SEG_7_ADDRESS   0x8000
#define ADC_ADDRESS     0xC000

// IO address pin
#define IOM     P3_4
#define IO 	    1
#define MEM 	0

// C/D pin (if not using A0)
#define CD      P3_5

// Remove for RTC addressing on low pins instead of A[9..12]
#define upperAddress

// Memory addresses
#define START_RAM 0x0000
#define END_RAM 0xFFFF

#endif

