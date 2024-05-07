#include "settings_programs.h"
#include "menu.h"
#include "io.h"
#include "rtc.h"
#include "lcdDriver.h"

void settings_set_time(void) {
    // in1 (month, hour), in2 (day, minute), in3 (year, second)
    uint8_t in1, in2, in3;
    menuTitle("Set Time");
    println("Input new time:");

    // Print placeholder time
    setTextColor(GRAY, BLACK);
    setCursor(0, 40);
    rtcPrint();

    // Get inputs from user
    setTextColor(WHITE, BLACK);
    setCursor(0, 40);
    // Get Date
    in1 = menuInputByteBCD();
    printChar('/');
    in2 = menuInputByteBCD();
    printChar('/');
    in3 = menuInputByteBCD();
    printChar(' ');
    // Set date
    rtcSetDate(in2, in1, in3);

    // Get Time
    in1 = menuInputByteBCD();
    printChar(':');
    in2 = menuInputByteBCD();
    printChar(':');
    in3 = menuInputByteBCD();
    // Set date
    rtcSetTime(in1, in2, in3);
    return;
}

void settings_set_time_auto(void) {
    return;
}

void settings_wifi(void) {

}

