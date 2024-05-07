#include "input.h"
#include "uart.h"

__bit inputKeyPressed;
__code uint8_t keys_lut[16] = {0x01,0x02,0x03,0x0A,0x04,0x05,0x06,0x0B,0x07,0x08,0x09,0x0C,0x0F,0x00,0x0E,0x0D};

uint8_t inputGetKey(void)
{
    __asm
    PUSH ACC
    PUSH B
    PUSH DPH
    RK_1 .EQU 00011
    RK_2 .EQU 00012
    RK_L1 .EQU 00013
    RK_3 .EQU 00014
    RK_4 .EQU 00015
    RK_5 .EQU 00016
    RK_L2 .EQU 00017
    ; Read rows
    MOV P1, #0x0F
    MOV A, P1
    ; Check for current keypress, otherwise return false
    CJNE A, #0x0F, RK_1$
    CLR _inputKeyPressed
    SJMP RK_4$
RK_1$:
    ; Check for prev keypress, if so return false, else
    ; note current one
    JB _inputKeyPressed, RK_4$
    SETB _inputKeyPressed
    ; Read column and combine with row info
    MOV B, A
    MOV P1, #0xF0
    MOV A, P1
    ORL A, B
    ; Parse Value
    ; Complement and save the number
    CPL A
    PUSH ACC
    SWAP A
    ; Loop to count how many 0s are at the start of row val,
    ; which translates to the row the keypress is in
    MOV B, #0
RK_L1$:
    RRC A
    JC RK_2$
    INC B
    SJMP RK_L1$
RK_2$:
    ; Multiply row counter by 4
    MOV A, B
    RL A
    RL A
    MOV B, A
    ; Get column value to lower nibble
    POP ACC
    ; Loop to count how many 0s are at the start of
    ; column val, adding it to row * 4
RK_L2$:
    RRC A
    JC RK_3$
    INC B
    SJMP RK_L2$
RK_3$:
    ; Index second LUT (Add failsafe?)
    MOV A, B
    ANL A, #0xF0
    JNZ RK_4$
    MOV A, B
    MOV DPTR, #_keys_lut
    MOVC A, @A+DPTR
    SJMP RK_5$
RK_4$:
    MOV A, #0x10
RK_5$:
    MOV DPL, A
    POP DPH
    POP B
    POP ACC
    __endasm;
    return DPL;
}

uint8_t inputGetKeyBlocking(void)
{
    uint8_t key;
    do {
        key = uart_receive();
        if (key == 0xFF) {
            key = inputGetKey();
        } else {
            // Convert from ascii to hex
            key -= '0';
            if (key > 9)
                key -= 7;
        }
    } while (key == 0x10);
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
