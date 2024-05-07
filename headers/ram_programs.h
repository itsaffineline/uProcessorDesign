#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "boarddefs.h"

/**
 * @brief hex dump on ram
 */
void prog_ram_dump(void);

/**
 * @brief performs a ram check
 */
void prog_ram_check(void);

/**
 * @brief edit the xdata ram
 */
void prog_ram_edit(void);

/**
 * @brief move a value from one address to another
 */
void prog_ram_move(void);

/**
 * @brief Find a value in the xdata RAM
 * @param count If true, it will count the number of occurrences before printing
 */
void prog_ram_find(uint8_t count);

#endif

