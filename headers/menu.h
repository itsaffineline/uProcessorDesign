#ifndef MENU_H
#define MENU_H

#include "boarddefs.h"

uint8_t menuCreate(__code char *title, __code char *options[], uint8_t optionCount) __reentrant;

uint8_t mainMenu(void);

void menuTitle(char *title);

uint8_t menuPromptByte(char *prompt);

uint16_t menuPromptWord(char *prompt);

uint8_t menuPromptDatatype(void);

uint8_t menuInputByte(void);

uint8_t menuInputByteBCD(void);

uint8_t menuInputDecimal(uint8_t digits);

void menuPrintAndWait(char *message);

#endif

