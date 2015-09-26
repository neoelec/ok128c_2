/* ========================================================================== */
/*                     Exp05_2.c : Key Input - Debouncing                     */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "   KEY INPUT    ");       // display title
    LCD_string(0xC0, "Press KEY4-KEY1!");

    while (1) {
        switch (Key_input()) {                  // key input
        case (0xF0 & ~_BV(PF4)):
            PORTB = _BV(PB4);
            LCD_string(0xC0, "  KEY1 is OK !  ");
            break;
        case (0xF0 & ~_BV(PF5)):
            PORTB = _BV(PB5);
            LCD_string(0xC0, "  KEY2 is OK !  ");
            break;
        case (0xF0 & ~_BV(PF6)):
            PORTB = _BV(PB6);
            LCD_string(0xC0, "  KEY3 is OK !  ");
            break;
        case (0xF0 & ~_BV(PF7)):
            PORTB = _BV(PB7);
            LCD_string(0xC0, "  KEY4 is OK !  ");
            break;
        default:
            break;
        }
    }

    return 0;
}
