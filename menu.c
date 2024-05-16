#include "menu.h"
#include "io.h"
#include "input.h"
#include "lcdDriver.h"
#include "rtc.h"
#include "ram_programs.h"
#include "settings_programs.h"
#include "games_programs.h"
#include "uart.h"

#define MAIN_MENU_COUNT 3
#define PROGRAM_COUNT 6
#define GAMES_COUNT 2
#define SETTING_COUNT 3

__code char* const mainMenuOptions[MAIN_MENU_COUNT] = {"(1) Memory Functions", "(2) Games", "(3) Settings"};
__code char* const programs[PROGRAM_COUNT] = {"(1) Memory Dump", "(2) RAM Check", "(3) RAM Move", "(4) RAM Edit", "(5) RAM Find", "(6) RAM Count"};
__code char* const games[GAMES_COUNT] = {"(1) Pong", "(2) Snake"};
__code char* const settings[SETTING_COUNT] = {"(1) Set Time (Manual)", "(2) Set Time (Automatic)", "(3) ADC Print"};

uint8_t menuCreate(__code char *title, __code char *options[], uint8_t optionCount) __reentrant {
    uint8_t i, selection = 0, keyInput, nesInput, nesEdgeInput;
    // Print title
    menuTitle(title);
    setForegroundColor(GRAY);
    setCursor(0, 40);
    println("(0) Back");
    // Init NES
    nesInput = 0;

    // Print programs
    for (i = 0; i < optionCount; i++) {
        // Update text color based on selection
        if (i == selection)
            setForegroundColor(WHITE);
        else
            setForegroundColor(GRAY);
        // Print program name
        setCursor(0, 60 + (i * 20));
        print(options[i]);
    }

    while (1) {
        // Wait for valid input
        do {
            // Key and UART
            keyInput = uart_receive();
            if (keyInput == 0xFF) {
                keyInput = inputGetKey(1);
            } else {
                // Convert from ascii to hex
                keyInput -= (keyInput >= '9') ? 0x37 : 0x30;
            }
            // NES input
            nesInput = inputGetNES();
        } while (keyInput > optionCount && nesInput == 0);

        // Set size back
        setTextSize(2);

        // If key pressed
        if (keyInput != 0xFF) {
            return keyInput;
        }

        // NES
        if (nesInput & NES_A) return selection + 1;
        if (nesInput & NES_B) return 0;

        // Rewrite old selection in gray
        setForegroundColor(GRAY);
        setCursor(0, 60 + (selection * 20));
        print(options[selection]);

        // NES Update selection
        if (nesInput & NES_UP && selection > 0) selection--;
        if (nesInput & NES_DOWN && selection < optionCount - 1) selection++;

        // Rewrite new selection in white
        setForegroundColor(WHITE);
        setCursor(0, 60 + (selection * 20));
        print(options[selection]);
    }
}

uint8_t mainMenu(void) {
    uint8_t selection;
    while (true) {
        selection = menuCreate("Main Menu", mainMenuOptions, MAIN_MENU_COUNT);
        switch (selection) {
            case 1:
                while (selection != 0) {
                    selection = menuCreate("Memory Functions", programs, PROGRAM_COUNT);
                    switch (selection) {
                        case 1:
                            prog_ram_dump();
                            break;
                        case 2:
                            prog_ram_check();
                            break;
                        case 3:
                            prog_ram_move();
                            break;
                        case 4:
                            prog_ram_edit();
                            break;
                        case 5:
                            prog_ram_find(0);
                            break;
                        case 6:
                            prog_ram_find(1);
                            break;
                    }
                }
                break;
            case 2:
                while (selection != 0) {
                selection = menuCreate("Games", games, GAMES_COUNT);
                    switch (selection) {
                        case 1:
                            games_pong();
                            break;
                        case 2:
                            games_snake();
                            break;
                    }
                }
                break;
            case 3:
                while (selection != 0) {
                    selection = menuCreate("Settings", settings, SETTING_COUNT);
                    switch (selection) {
                        case 1:
                            settings_set_time();
                            break;
                        case 2:
                            settings_set_time_auto();
                            break;
                        case 3:
                            settings_wifi();
                            break;
                    }
                }
                break;
        }
    }
}

void menuTitle(char *title) {
    setForegroundColor(WHITE);
    fillScreen();
    setCursor(0,0);
    setTextSize(3);
    print(title);
    printChar('\n');
    setTextSize(2);
    return;
}

uint8_t menuPromptByte(char *prompt) {
    uint8_t data;
    print(prompt);
    print(": 0x");
    data = menuInputByte();
    printChar('\n');
    return data;
}

uint16_t menuPromptWord(char *prompt) {
    uint16_t data;
    print(prompt);
    print(": 0x");
    data = menuInputByte() << 8 | menuInputByte();
    printChar('\n');
    return data;
}

uint8_t menuPromptDatatype(void) {
    uint8_t key;
    print("Enter datatype\n1-Byte, 2-Word, 4-DWord: ");
    do {
        key = inputGetKeyBlocking();
    } while (key < 0x01 || key > 0x04 || key == 0x03);
    printNibbleHexadecimal(key);
    print("\n");
    return key;
}

uint8_t menuInputByte(void) {
    uint8_t key;
    key = inputGetKeyBlocking();
    printNibbleHexadecimal(key);
    key = key << 4;
    key |= inputGetKeyBlocking();
    printNibbleHexadecimal(key);
    return key;
}

uint8_t menuInputByteBCD(void) {
    uint8_t key, key2;
    do {
        key = inputGetKeyBlocking();
    } while (key > 0x09);
    printNibbleHexadecimal(key);
    key = key << 4;
    do {
        key2 = inputGetKeyBlocking();
    } while (key2 > 0x09);
    printNibbleHexadecimal(key2);
    return key | key2;
}

uint8_t menuInputDecimal(uint8_t digits) {
    uint8_t key, i, data = 0;
    for (i = 0; i < digits; i++) {
        do {
            key = inputGetKeyBlocking();
        } while (key > 0x09);
        printNibbleHexadecimal(key);
        data = data * 10 + key;
    }
    return data;
}

void menuPrintAndWait(char *message) {
    print(message);
    print("\nPress any key...");
    inputGetKeyBlocking();
    return;
}

