#include "timer.h"

__bit elapsed = 0;

void timerInit(void) {
    // Set timer 0 to 16-bit mode
    TMOD = 0x01;
    // Set timer 0 to 10ms
    TH0 = 0xB1;
    TL0 = 0xE0;
    TR0 = 1;
    // Enable interrupts
    ET0 = 1;
    EA = 1;
}

void timerISR(void) __interrupt (1) __naked {
    __asm
    MOV TL0, #0xE0
    MOV TH0, #0xB1
    SETB _elapsed
    RETI
    __endasm;
}

bool has10msElapsed(void) {
    __asm
    PUSH ACC
    MOV A.0, _elapsed
    MOV DPL, A
    POP ACC
    __endasm;
    return DPL;
}
