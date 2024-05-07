#include "io.h"

inline void iowrite8(uint8_t __xdata* address, uint8_t d) {
    IOM = 1;
    *address = d;
    IOM = 0;
}

inline uint8_t ioread8(uint8_t __xdata* address) {
    uint8_t d;
    IOM = 1;
    d = *address;
    IOM = 0;
    return d;
}

bool check_dptr_overflow(uint16_t addr, uint8_t datatype, uint16_t blocksize) {
    if (blocksize & ~(0xFFFF >> (datatype / 2))) {
        return true;
    }
    else {
        return (0xFFFF - blocksize * datatype + 1) < addr;
    }
}

