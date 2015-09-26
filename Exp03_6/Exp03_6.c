/* ========================================================================== */
/*                    Exp03_6.c : Infinite Loop(2) - Repeat                   */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    uint8_t i, LED;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, " Infinite Loop  ");       // display title
    LCD_string(0xC0, "     Repeat     ");

    while (1) {                                 // infinite loop
        LED = _BV(PB4);                         // shift from LED1 to LED4
        for (i = 1; i <= 4; i++) {
            PORTB = LED;                        // output LED data
            Delay_ms(500);
            LED <<= 1;                          // shift left
        }
    }

    return 0;
}
