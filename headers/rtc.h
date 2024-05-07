#ifndef RTC_H
#define RTC_H

#include "boarddefs.h"

// RTC address registers
#ifdef upperAddress
	#define __RTC_SECONDS_REG__                 RTC_ADDRESS + (0x00 << 9) 
	#define __RTC_MINUTES_REG__                 RTC_ADDRESS + (0x01 << 9)
	#define __RTC_HOURS_REG__                	RTC_ADDRESS + (0x02 << 9)
	#define __RTC_DAYS_REG__ 		            RTC_ADDRESS + (0x03 << 9)
	#define __RTC_DATE_REG__        	        RTC_ADDRESS + (0x04 << 9)
	#define __RTC_MONTH_REG__           	    RTC_ADDRESS + (0x05 << 9)
	#define __RTC_YEAR_REG__                	RTC_ADDRESS + (0x06 << 9)
	#define __RTC_CENTURY_REG__                	RTC_ADDRESS + (0x07 << 9)
	#define __RTC_ALARM_SEC_REG__               RTC_ADDRESS + (0x08 << 9)
	#define __RTC_ALARM_MIN_REG__               RTC_ADDRESS + (0x09 << 9)
	#define __RTC_ALARM_HOUR_REG__              RTC_ADDRESS + (0x0A << 9)
	#define __RTC_ALARM_DAY_REG__               RTC_ADDRESS + (0x0B << 9)
	#define __RTC_WATCHDOG1_REG__               RTC_ADDRESS + (0x0C << 9)
	#define __RTC_WATCHDOG2_REG__               RTC_ADDRESS + (0x0D << 9)
	#define __RTC_CTRL_A_REG__                  RTC_ADDRESS + (0x0E << 9)
	#define __RTC_CTRL_B_REG__                  RTC_ADDRESS + (0x0F << 9)
	#define __RTC_RAM_ADDR_REG__				RTC_ADDRESS + (0x10 << 9)
	#define __RTC_EXT_RAM_ADDR_REG__			RTC_ADDRESS + (0x13 << 9)
#else
	#define __RTC_SECONDS_REG__                 RTC_ADDRESS + 0x00
	#define __RTC_MINUTES_REG__                 RTC_ADDRESS + 0x01
	#define __RTC_HOURS_REG__                	RTC_ADDRESS + 0x02
	#define __RTC_DAYS_REG__ 		            RTC_ADDRESS + 0x03
	#define __RTC_DATE_REG__        	        RTC_ADDRESS + 0x04
	#define __RTC_MONTH_REG__           	    RTC_ADDRESS + 0x05
	#define __RTC_YEAR_REG__                	RTC_ADDRESS + 0x06
	#define __RTC_CENTURY_REG__                	RTC_ADDRESS + 0x07
	#define __RTC_ALARM_SEC_REG__               RTC_ADDRESS + 0x08
	#define __RTC_ALARM_MIN_REG__               RTC_ADDRESS + 0x09
	#define __RTC_ALARM_HOUR_REG__              RTC_ADDRESS + 0x0A
	#define __RTC_ALARM_DAY_REG__               RTC_ADDRESS + 0x0B
	#define __RTC_WATCHDOG1_REG__               RTC_ADDRESS + 0x0C
	#define __RTC_WATCHDOG2_REG__               RTC_ADDRESS + 0x0D
	#define __RTC_CTRL_A_REG__                  RTC_ADDRESS + 0x0E
	#define __RTC_CTRL_B_REG__                  RTC_ADDRESS + 0x0F
	#define __RTC_RAM_ADDR_REG__				RTC_ADDRESS + 0x10
	#define __RTC_EXT_RAM_ADDR_REG__			RTC_ADDRESS + 0x13

#endif

// Control A Register Values
#define __CTRLA_WDF_SET__		0x02
#define __CTRLA_KSF_SET__ 		0x04
#define __CTRLA_TDF_SET__ 		0x08
#define __CTRLA_PAB_SET__		0x10
#define __CTRLA_PRS_SET__		0x20

// Control B Register Values
#define __CTRLB_WDS_SET__ 		0x01
#define __CTRLB_WDE_SET__ 		0x02
#define __CTRLB_KIE_SET__ 		0x04
#define __CTRLB_TIE_SET__ 		0x08
#define __CTRLB_TPE_SET__ 		0x10
#define __CTRLB_BME_SET__ 		0x20
#define __CTRLB_CS_SET__ 		0x40
#define __CTRLB_TE_SET__ 		0x80

//Month Control Bits
#define __MON_EOSC_SET__ 		0x80
#define __MON_E32K_SET__ 		0x40
#define __MON_BB32_SET__ 		0x20

//Masks
#define __RTC_MONTH_MASK__ 		0x1F
#define __RTC_DATE_MASK__ 		0x3F
#define __RTC_SECONDS_MASK__  	0x7F
#define __RTC_MINUTES_MASK__    0x7F
#define __RTC_HOURS_MASK__		0x3F
#define __RTC_DAYS_MASK__		0x07
#define __RTC_YEAR_MASK__		0xFF
#define __RTC_CENTURY_MASK__	0xFF

/** Initialize the RTC */
void rtcInit(void);

uint8_t rtcBcdToAscii(uint8_t bcd);

uint8_t rtcAsciiToBcd(uint8_t ascii);

void rtcRegToLCD(uint8_t __xdata * reg, uint8_t mask);

void rtcPrint(void);

void rtcSetTime(uint8_t hour, uint8_t minute, uint8_t second);

void rtcSetDate(uint8_t day, uint8_t month, uint8_t year);

#endif
