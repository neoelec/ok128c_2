/* ========================================================================== */
/*                       Exp20_3.c : Watchdog Timer (2)                       */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    uint8_t i;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module

    LCD_string(0x80, "  OK-128  V3.0  ");       // display title
    LCD_string(0xC0, " Watchdog Timer ");
    PORTE = _BV(PE2);                           // buzzer on
    PORTB = _BV(PB4);                           // LED1 on
    Delay_ms(200);
    PORTB = _BV(PB5);                           // LED2 on
    Delay_ms(200);
    PORTB = _BV(PB6);                           // LED3 on
    Delay_ms(200);
    PORTB = _BV(PB7);                           // LED4 on
    Delay_ms(200);
    PORTB = 0x00;                               // all LED off
    PORTE = 0x00;                               // buzzer off
    Delay_ms(1000);

    WDTCR = _BV(WDCE) | _BV(WDE);               // enable Watchdog Timer
    WDTCR = _BV(WDE) | _BV(WDP2);               // timeout = 0.22 sec

    LCD_string(0x80, "Press KEY4/KEY1!");       // display command
    LCD_string(0xC0, "(KEY3/2=ON/OFF) ");
    PORTB = _BV(PB6);                           // LED3 on

    while (1) {
        switch (Key_input()) {                  // key input
        case (0xF0 & ~_BV(PF4)):
            LCD_string(0x80, "                ");   // KEY1
            LCD_string(0xC0, "   Blink LED1   ");
            for (i = 0; i < 20; i++) {
                asm volatile ("wdr");
                PORTB ^= _BV(PB4);
                Delay_ms(100);
            }
            LCD_string(0x80, "Press KEY4/KEY1!");
            LCD_string(0xC0, "                ");
            break;
        case (0xF0 & ~_BV(PF7)):
            LCD_string(0x80, "                ");   // KEY4
            LCD_string(0xC0, "   Blink LED4   ");
            for (i = 0; i < 10; i++) {
                asm volatile ("wdr");
                PORTB ^= _BV(PB7);
                Delay_ms(500);
            }
            LCD_string(0x80, "Press KEY4/KEY1!");
            LCD_string(0xC0, "                ");
            break;
        case (0xF0 & ~_BV(PF5)):
            WDTCR = _BV(WDCE) | _BV(WDE);       // KEY2
            WDTCR = 0x00;
            PORTB = _BV(PB5);
            break;
        case (0xF0 & ~_BV(PF6)):
            WDTCR = _BV(WDCE) | _BV(WDE);       // KEY3
            WDTCR = _BV(WDE) | _BV(WDP2);
            PORTB = _BV(PB6);
            break;
        default:
            asm volatile ("wdr");
            break;
        }
    }

    return 0;
}
