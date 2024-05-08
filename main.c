#include "main.h"
#include "rtc.h"
#include "io.h"
#include "input.h"
#include "lcdDriver.h"
#include "menu.h"
#include "uart.h"
#include "timer.h"

__sfr __at(0x8E) AUXR;

void main (void) {
    AUXR = 0x02;
    CD = CMD;
    IOM = IO;
    iowrite8((__xdata uint8_t*)SEG_7_ADDRESS, 0x00);
    uart_init();
    timerInit();
    tftLcdInit();
    setRotation(3);
    rtcInit();
    mainMenu();
}

