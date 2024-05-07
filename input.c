#include "input.h"
#include "uart.h"

__bit inputKeyPressed;
uint8_t button;
//__code uint8_t keys_lut[16] = {0x01,0x02,0x03,0x0A,0x04,0x05,0x06,0x0B,0x07,0x08,0x09,0x0C,0x0F,0x00,0x0E,0x0D};
__code uint8_t rawhex[] = {0x7D, 0xEE, 0xED, 0xEB, 0xDE, 0xDD, 0xDB, 0xBE, 0xBD, 0xBB, 0xE7, 0xD7, 0xB7, 0x77, 0x7B, 0x7E, 0x00};

uint8_t inputGetKey(void)
{
    uint8_t decode = 0x10;
	while(1)
	{
		uint8_t colloc, rowloc, i = 0;
		do
		{
		P1 = 0xF0;
		colloc = P1;
		colloc&= 0xF0;		/* mask port for column read only */
		}while(colloc == 0xF0);		/* read status of column */
		P1 = 0x0F;
		rowloc = P1;
		rowloc &= 0x0F;
		button = colloc | rowloc;
		while(rowloc != 0x0F)
		{
			P1 = 0x0F;
			rowloc = P1;
		}
		while(i<16)
		{
			if (rawhex[i] == button)
			{
				decode = i;
				return decode;
			}
			i++;
		}
	}
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
