/* ========================================================================== */
/*                      Exp02_2a.c : Basic C Program (2)                      */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module

    LCD_string(0x80, "  OK-128  V3.0  ");       // display logo

    while (1) {
        LCD_string(0xC0, " ATmega128-16AU ");   // display message 1
        Delay_ms(1000);
        LCD_string(0xC0, "   2006/05/01   ");   // display message 2
        Delay_ms(1000);
    }

    return 0;
}
