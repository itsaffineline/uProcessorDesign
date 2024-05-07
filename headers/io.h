#ifndef IO_H
#define IO_H

#include "boarddefs.h"
#include "bool.h"

/* LCD control pin interfacing. */
#define IOM P3_4
#define IO 	1
#define MEM 0

inline void iowrite8(uint8_t __xdata* address, uint8_t d);

inline uint8_t ioread8(uint8_t __xdata* address);

bool check_dptr_overflow(uint16_t addr, uint8_t datatype, uint16_t blocksize);

#endif
