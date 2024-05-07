#ifndef UART_H
#define UART_H

#include "boarddefs.h"
#include "bool.h"

__sfr __at 0xCA RCAP2L;
__sfr __at 0xCB RCAP2H;
__sfr __at 0xC8 T2CON;

struct socket {
    uint8_t port;
    uint8_t status;
    uint8_t* tx_data;
    __xdata uint8_t* rx_data;
    uint8_t tx_len;
    uint8_t rx_len;
};

#define MAX_SOCKETS 4
#define SOCKET_OPEN 0x01
#define SOCKET_TX_READY 0x02
#define SOCKET_RX_READY 0x03

/**
 * @brief Initialize UART with 8 bit data, 1 stop bit, no parity
 * At 57600 baud
 */
void uart_init(void);

void uart_isr(void) __interrupt(4);

/**
 * @brief Send raw bytes over uart
 * 
 * @param data Pointer to data to send
 * @param len Length of data to send
 */
void uart_send(uint8_t* data, uint8_t len);

/**
 * @brief Determine if the UART is ready to send more data
 *
 * @return true if the UART is ready to send more data
 */
bool uart_is_send_complete(void);

/**
 * @brief Receive a byte from the UART
 *
 * @return The byte received, or 0xFF if no byte is available
 */
uint8_t uart_receive(void);

/**
 * @brief Receive a byte from the UART
 *
 * @return The byte received
 */
uint8_t uart_receive_blocking(void);

#endif

