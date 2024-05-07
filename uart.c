#include "uart.h"

uint8_t* uart_tx;
uint8_t uart_tx_len = 0;
uint8_t uart_rx = 0xFF;

/*
__xdata struct socket sockets[MAX_SOCKETS];
uint8_t tx_socket, tx_index;
uint8_t rx_socket, rx_index;
__bit transmitting;
__bit receiving;
*/

void uart_init(void)
{
    // Set UART for 8 data, 1 stop bit, no parity
    // Also set Reception enable
    SCON = 0x50;
    // Set baud rate to 57600
    RCAP2H = 0xFF;
    RCAP2L = 0xF3;
    // Enable Timer 2
    T2CON = 0x34;
    // Enable UART interrupt
    EA = 1;
    ES = 1;
}

void uart_isr(void) __interrupt(4)
{
    // Check if the interrupt is caused by a received byte
    if (RI)
    {
        // Clear the interrupt flag
        RI = 0;
        // Read the received byte
        uart_rx = SBUF;
    }
    // Check if the interrupt is caused by a transmitted byte
    if (TI)
    {
        // Check if there are more bytes to be transmitted
        if (uart_tx_len-- > 0)
        {
            // Set the next byte to be transmitted
            SBUF = *(++uart_tx);
        }
        // Clear the interrupt flag
        TI = 0;
    }
}

void uart_send(uint8_t* data, uint8_t len)
{
    // Set the data to be transmitted
    uart_tx = data;
    // Set the length of the data to be transmitted
    uart_tx_len = len - 1;
    // Start the transmission
    SBUF = *uart_tx;
    uart_tx++;
}

bool uart_is_send_complete(void)
{
    return uart_tx_len == 0;
}

uint8_t uart_receive(void)
{
    uint8_t rx = uart_rx;
    uart_rx = 0xFF;
    return rx;
}

uint8_t uart_receive_blocking(void)
{
    uint8_t rx;
    // Wait for a byte to be received
    do
    {
        rx = uart_rx;
    } while (rx == 0xFF);
    // Clear the received byte
    uart_rx = 0xFF;
    return rx;
}

