/* ------------------------------------------------------------------------------- */
/*  OK128.h : I/O Address Definition & User Function for OK-128 V3.0 Training Kit  */
/* ------------------------------------------------------------------------------- */

#ifndef __OK128_H__
#define __OK128_H__

#include <stdint.h>

#define LCD_DATABUS     (*(volatile uint8_t *)0x2000)   // LCD/GLCD data
                                                        // 7-segment LED pattern
#define LCD_CONTROL     (*(volatile uint8_t *)0x2100)   // LCD/GLCD control signal
#define DIG_SELECT      (*(volatile uint8_t *)0x2200)   // 7-segment LED digit select
                                                        // 8-bit parallel data

#define RTC_SECOND      (*(volatile uint8_t *)0x2300)   // DS12C887 RTC
#define RTC_SEC_ALM     (*(volatile uint8_t *)0x2301)
#define RTC_MINUTE      (*(volatile uint8_t *)0x2302)
#define RTC_MIN_ALM     (*(volatile uint8_t *)0x2303)
#define RTC_HOUR        (*(volatile uint8_t *)0x2304)
#define RTC_HOUR_ALM    (*(volatile uint8_t *)0x2305)
#define RTC_WEEKDAY     (*(volatile uint8_t *)0x2306)
#define RTC_DATE        (*(volatile uint8_t *)0x2307)
#define RTC_MONTH       (*(volatile uint8_t *)0x2308)
#define RTC_YEAR        (*(volatile uint8_t *)0x2309)
#define RTC_A           (*(volatile uint8_t *)0x230A)
#define RTC_B           (*(volatile uint8_t *)0x230B)
#define RTC_C           (*(volatile uint8_t *)0x230C)
#define RTC_D           (*(volatile uint8_t *)0x230D)
#define RTC_FLAG        (*(volatile uint8_t *)0x230F)

void MCU_initialize(void);

void Delay_us(uint8_t time_us);

void Delay_ms(uint16_t time_ms);

void Beep(void);

void LCD_command(uint8_t command);

void LCD_data(uint8_t data);

void LCD_string(uint8_t command, char *string);

void LCD_initialize(void);

uint8_t Key_input(void);

#endif /* __OK128_H__ */
