#include "ram_programs.h"
#include "lcdDriver.h"
#include "menu.h"
#include "io.h"
#include "input.h"

#define ITEMS_PER_PAGE 16

/**
 * @brief Dumps a value from an xdata location to the LCD
 *
 * @param addr The address of the value
 * @param datatype The size in bytes of the value
 * @param maxPrints The maximum number of values to print
 */
void _dump_loc(__xdata uint8_t* addr, uint8_t datatype, uint8_t maxPrints);

/**
 * @brief Finds the next instance of a byte in a block of memory
 *
 * @param addr The address to start searching from
 * @param eaddr The end address of the block
 * @param find The byte to find
 * @return The address of the next instance of the byte
 */
__xdata uint8_t* _find_next(__xdata uint8_t* addr, uint8_t eaddr, uint8_t find);

/**
 * @brief Checks if ram is working by writing a byte to all memory and checking it
 *
 * @param testByte The byte to test
 * @return 1 if the test fails, 0 otherwise
 */
uint8_t _ram_check(uint8_t testByte);

void prog_ram_dump(void) {
    menuTitle("RAM Dump");
    // Get inputs from user
    uint16_t startAddress = menuPromptWord("Start Address");
    uint8_t datatype = menuPromptDatatype();
    uint16_t blockSize = menuPromptWord("Block Size");
    // Check for overflowing block size
    if (check_dptr_overflow(startAddress, datatype, blockSize)) {
        menuPrintAndWait("Address Overflow");
        return;
    }

    // Setup tabulator
    menuTitle("RAM Dump");
    setTextSize(1);

    // Start tabulation
    uint16_t endAddress = startAddress + (datatype * (blockSize - 1));
    __xdata uint8_t* currentAddress = (__xdata uint8_t*)startAddress;
    uint8_t maxPrints;
    const uint8_t bytesPerPage = datatype * ITEMS_PER_PAGE;

    // Tabulation loop
    while (true) {
        // Calculate maximum number of prints
        if (endAddress - (uint16_t)currentAddress < bytesPerPage) {
            maxPrints = (endAddress - (uint16_t)currentAddress) / datatype + 1;
        } else {
            maxPrints = ITEMS_PER_PAGE;
        }
        // Print the current data
        _dump_loc(currentAddress, datatype, maxPrints);
        // Clear the rest of the screen
        if (maxPrints < ITEMS_PER_PAGE) {
            fillRect(getCursorX(), getCursorY(), 6 * (8 + datatype * 3), 10 * (ITEMS_PER_PAGE - maxPrints), BLACK);
        }
        switch (inputGetKeyBlocking()) {
            case 0:
                return;
            // go left
            case 0xF:
                if ((uint16_t)currentAddress > startAddress)
                    currentAddress -= bytesPerPage;
                break;
            // go right
            case 0xE:
                if ((uint16_t)currentAddress + bytesPerPage < endAddress && (uint16_t)currentAddress < -bytesPerPage) {
                    currentAddress += bytesPerPage;
                } else {
                    menuPrintAndWait("\nAddress Overflow");
                    return;
                }
                break;
        }
    }
}

void _dump_loc(__xdata uint8_t* addr, uint8_t datatype, uint8_t maxPrints) {
    // Print the address
    setCursor(0, 30);
    // Print the next 20 datatypes
    for (uint8_t i = 0; i < maxPrints; i++) {
        print("0x");
        printByteHexadecimal((uint8_t)((uint16_t)addr >> 8));
        printByteHexadecimal((uint8_t)addr);
        printChar(':');
        // Print the data
        for (uint8_t j = 0; j < datatype; j++) {
            printByteHexadecimal(addr[j]);
        }
        // Print ascii
        printChar(' ');
        for (uint8_t j = 0; j < datatype; j++) {
            printChar((char)addr[j]);
        }
        // Move to the next data
        addr += datatype;
        printChar('\n');
    }
    return;
}

void prog_ram_check(void) {
    uint8_t tester;
    menuTitle("RAM Check");
    // Get test byte
    tester = menuPromptByte("Enter a test byte");
    // Write test byte to all memory and check as you go
    if (_ram_check(tester) || _ram_check(~tester)) {
        menuPrintAndWait("RAM Check Failed");
    } else {
        menuPrintAndWait("RAM Check Passed");
    }
    return;
}

uint8_t _ram_check(uint8_t testByte) {
    // Write test byte to all memory and check as you go
    uint8_t backup;
    volatile __xdata uint8_t* mem = (__xdata uint8_t*)0;
    // Loop through all memory
    do {
        // Backup the current byte and write the test byte
        backup = *mem;
        *mem = testByte;
        // Check if the test byte was written correctly
        if (*((__xdata uint8_t*)((uint16_t)mem)) != testByte) {
            return 1;
        }
        // Restore the original byte
        *mem = backup;
    } while (++mem != 0);
    return 0;
}

void prog_ram_move(void) {
    menuTitle("RAM Move");
    // Get inputs from user
    uint16_t startAddress = menuPromptWord("Start Address");
    uint8_t datatype = menuPromptDatatype();
    uint16_t blockSize = menuPromptWord("Block Size");
    uint16_t destinationAddress = menuPromptWord("Destination Address");
    // Check for overflowing block size
    if (check_dptr_overflow(startAddress, datatype, blockSize)
            || check_dptr_overflow(destinationAddress, datatype, blockSize)) {
        menuPrintAndWait("Address Overflow");
        return;
    }
    // Copy block
    print("Move in progress...\n");
    __xdata uint8_t* src = (__xdata uint8_t*)startAddress;
    __xdata uint8_t* dst = (__xdata uint8_t*)destinationAddress;
    for (uint16_t i = 0; i < blockSize * datatype; i++) {
        dst[i] = src[i];
    }
    menuPrintAndWait("Move Successful");
    return;
}

void prog_ram_edit(void) {
    menuTitle("RAM Edit");
    // Get inputs from user
    uint16_t startAddress = menuPromptWord("Start Address");
    // Begin tabulation
    menuTitle("RAM Edit");
    __xdata uint8_t* loc = (__xdata uint8_t*)startAddress;
    while (true) {
        // Print the current data
        _dump_loc(loc, 1, 1);
        switch (inputGetKeyBlocking()) {
            case 0:
                return;
            // Edit byte
            case 0xF:
                setCursor(84, 30);
                *loc = menuInputByte();
                break;
            // go right
            case 0xE:
                loc++;
                if (loc == 0) {
                    menuPrintAndWait("\nAddress Overflow");
                    return;
                }
                break;
        }
    }
}

void prog_ram_find(uint8_t count) {
    menuTitle("RAM Find");
    // Get inputs from the user
    uint16_t startAddress = menuPromptWord("Start Address");
    uint16_t blockSize = menuPromptWord("Block Size");
    // Check for overflowing block size
    if (check_dptr_overflow(startAddress, 1, blockSize)) {
        menuPrintAndWait("Address Overflow");
        return;
    }
    uint8_t tester = menuPromptByte("Input byte to find");
    __xdata uint8_t* currentAddress = (__xdata uint8_t*)startAddress;
    // Count the number of times the byte is found
    if (count) {
        print("Searching...\n");
        uint16_t count = 0;
        for (uint16_t i = 0; i < blockSize; i++) {
            if (currentAddress[i] == tester) count++;
        }
        print("Matches: 0x");
        printByteHexadecimal((uint8_t)(count >> 8));
        printByteHexadecimal((uint8_t)count);
        menuPrintAndWait("");
    }
    // Page through the matches
    menuTitle("RAM Find");
    blockSize = (uint16_t)currentAddress + blockSize - 1;
    currentAddress = _find_next(currentAddress, blockSize, tester);
    while (true) {
        if ((uint16_t)currentAddress == blockSize + 1) {
            menuPrintAndWait("No more matches");
            return;
        }
        _dump_loc(currentAddress, 1, 1);
        switch (inputGetKeyBlocking()) {
            case 0:
                return;
            // Find next match
            case 0xE:
                currentAddress = _find_next(currentAddress + 1, blockSize, tester);
                break;
        }
    }
}

__xdata uint8_t* _find_next(__xdata uint8_t* addr, uint8_t eaddr, uint8_t find) {
    while (*addr != find) {
        if ((uint16_t)(++addr) == eaddr + 1) break;
    }
    return addr;
}

