#include "rtc.h"
#include "io.h"
#include "lcdDriver.h"

uint8_t old_seconds = 0;

/** Initialize the RTC */
void rtcInit(void)
{
    uint8_t ctrl_a =  __CTRLA_PAB_SET__;
    uint8_t ctrl_b = (__CTRLB_TE_SET__ | __CTRLB_BME_SET__);
    iowrite8((uint8_t __xdata *)(__RTC_CTRL_A_REG__), ctrl_a); //no interrupt operation
    iowrite8((uint8_t __xdata *)(__RTC_CTRL_B_REG__), ctrl_b);
    //rtcSetDate(0x21, 0x03, 0x23); //Set date to 21st March 2023
    //rtcSetTime(0x13, 0x30, 0x00); //Set time to 1:30:00 PM
}

void rtcSetTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    iowrite8((uint8_t __xdata *)(__RTC_HOURS_REG__), hour);
    iowrite8((uint8_t __xdata *)(__RTC_MINUTES_REG__), minute);
    iowrite8((uint8_t __xdata *)(__RTC_SECONDS_REG__), second);
}

void rtcSetDate(uint8_t day, uint8_t month, uint8_t year)
{
    iowrite8((uint8_t __xdata *)(__RTC_MONTH_REG__), __MON_E32K_SET__ | month);
    iowrite8((uint8_t __xdata *)(__RTC_DATE_REG__), day);
    iowrite8((uint8_t __xdata *)(__RTC_YEAR_REG__), year);
}

uint8_t rtcBcdToAscii(uint8_t bcd)
{
    bcd = bcd & 0x0F;
    if (bcd < 10)
    {
        bcd = 0x30 + bcd;
    }
    return bcd;
}

uint8_t rtcAsciiToBcd(uint8_t ascii)
{
    if(ascii >= 0x30 && ascii < 0x3A)
    {
        ascii = ascii & 0x0F;
    }
    return ascii;
}

void rtcRegToLCD(uint8_t __xdata * reg, uint8_t mask)
{
    uint8_t rtcTemp,rtcCurrent;
    rtcCurrent = ioread8(reg);
    rtcCurrent = rtcCurrent & mask;
    rtcTemp = rtcCurrent >> 4;
    printChar(rtcBcdToAscii(rtcTemp));
    printChar(rtcBcdToAscii(rtcCurrent));
}

void rtcPrint(void)
{
    uint8_t current_seconds = ioread8((uint8_t __xdata *)(__RTC_SECONDS_REG__));
    if (old_seconds == current_seconds)
    {
        return;
    }
    uint8_t __xdata * ptr = (uint8_t __xdata *)(__RTC_MONTH_REG__);
    rtcRegToLCD(ptr, 0x1F);
    printChar('/');
    ptr = (uint8_t __xdata *)(__RTC_DATE_REG__);
    rtcRegToLCD(ptr, 0x3F);
    printChar('/');
    ptr = (uint8_t __xdata *)(__RTC_YEAR_REG__);
    rtcRegToLCD(ptr, 0xFF);
    printChar(' ');
    ptr = (uint8_t __xdata *)(__RTC_HOURS_REG__);
    rtcRegToLCD(ptr, 0x3F);
    printChar(':');
    ptr = (uint8_t __xdata *)(__RTC_MINUTES_REG__);
    rtcRegToLCD(ptr, 0x7F);
    printChar(':');
    ptr = (uint8_t __xdata *)(__RTC_SECONDS_REG__);
    rtcRegToLCD(ptr,0x7F);
    old_seconds = current_seconds;
}
