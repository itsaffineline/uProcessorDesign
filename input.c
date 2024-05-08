#include "input.h"
#include "uart.h"
#include "timer.h"

__bit inputKeyPressed;
__code uint8_t keys_lut[16] = {0x01,0x02,0x03,0x0A,0x04,0x05,0x06,0x0B,0x07,0x08,0x09,0x0C,0x0F,0x00,0x0E,0x0D};
__code uint8_t keyDetect[] = {0x7D, 0xEE, 0xED, 0xEB, 0xDE, 0xDD, 0xDB, 0xBE, 0xBD, 0xBB, 0xE7, 0xD7, 0xB7, 0x77, 0x7B, 0x7E};

uint8_t inputGetKey(void)
{
    uint8_t key;
    // Read rows
    P1 = 0x0F;
    key = P1;
    if (key == 0x0F) {
        inputKeyPressed = 0;
        return 0xFF;
    }
    // Handle key release
    if (inputKeyPressed) {
        return 0xFF;
    }
    inputKeyPressed = 1;
    // Read column and combine with row info
    P1 = 0xF0;
    key |= P1;
    // Determine key
    for (uint8_t i = 0; i < 16; i++) {
        if (key == keyDetect[i]) {
            return i;
        }
    }
    return 0xFF;
}

uint8_t inputGetKeyBlocking(void)
{
    uint8_t key;
    do {
        while (!has10msElapsed());
        key = uart_receive();
        if (key == 0xFF) {
            key = inputGetKey();
        } else {
            // Convert from ascii to hex
            key -= (key >= '9') ? 0x37 : 0x30;
        }
    } while (key == 0xFF);
    return key;
}

uint8_t inputGetTouch(void)
{
    return 0x00;
}

uint8_t inputGetNES(void) __naked
{
    __asm
    DATALOOP .EQU 00001
    PUSH ACC
    PUSH PSW
    ; Pulse latch
    SETB P3.5
    CLR P3.5
    ; Setup for reading
    CLR A
    MOV R7, #7
    ; Get bit
    MOV C, P3.3
    RRC A
    ; Loop
DATALOOP$:
    ; Pulse clock
    SETB P3.2
    CLR P3.2
    ; Get bit
    MOV C, P3.3
    RRC A
    DJNZ R7, DATALOOP$
    ; Return
    CPL A
    MOV DPL, A
    POP PSW
    POP ACC
    RET
    __endasm;
}
