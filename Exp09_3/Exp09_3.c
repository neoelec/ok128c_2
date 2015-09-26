/* ========================================================================== */
/*                       Exp09_3.c : 119 Ambulance Sound                      */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    uint8_t i, glitch;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module

    LCD_string(0x80, "   * SOUND *    ");       // display title
    LCD_string(0xC0, " 119 Ambulance  ");

    TCCR3A = _BV(COM3A0);                       // CTC mode(12), use OC3A
    TCCR3B = _BV(WGM33) | _BV(WGM32);           // f(OC3A) = 16MHz/8/2/(1+N)
    TCCR3C = 0x00;
    TCNT3 = 0x0000;                             // clear Timer/Counter3

    while (1) {
        for (i = 0; i < 5; i++) {
            TCCR3B |= _BV(CS32);                // speaker on
            do {
                glitch = TCNT3L;                // protect glitch
                glitch = TCNT3H;
            } while (glitch >= (2082 >> 8));
            ICR3 = 2082;                        // 480 Hz
            Delay_ms(500);                      // delay for 500 ms

            do {
                glitch = TCNT3L;                // protect glitch
                glitch = TCNT3H;
            } while (glitch >= (2777 >> 8));
            ICR3 = 2777;                        // 360 Hz
            Delay_ms(500);                      // delay for 500 ms
        }

        TCCR3B &= ~_BV(CS32);                   // speaker off
        Delay_ms(1000);                         // delay for 1 sec
    }

    return 0;
}
