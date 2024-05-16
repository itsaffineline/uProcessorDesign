#ifndef INPUT_H
#define INPUT_H

#include "boarddefs.h"

#define NES_A 0x01
#define NES_B 0x02
#define NES_SELECT 0x04
#define NES_START 0x08
#define NES_UP 0x10
#define NES_DOWN 0x20
#define NES_LEFT 0x40
#define NES_RIGHT 0x80

uint8_t inputGetKey(uint8_t rtc);

uint8_t inputGetKeyBlocking(void);

uint8_t inputGetTouch(void);

uint8_t inputGetNES(void);

#endif
