/* ========================================================================== */
/*                      Exp09_2.c : Telephone Bell Sound                      */
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
    LCD_string(0xC0, " Telephone Bell ");

    TCCR3A = _BV(COM3A0);                       // CTC mode(12), use OC3A
    TCCR3B = _BV(WGM33) | _BV(WGM32);           // f(OC3A) = 16MHz/8/2/(1+N)
    TCCR3C = 0x00;
    TCNT3 = 0x0000;                             // clear Timer/Counter3

    while (1) {
        for (i = 0; i < 20; i++) {
            TCCR3B |= _BV(CS32);                // speaker on
            do {
                glitch = TCNT3L;                // protect glitch
                glitch = TCNT3H;
            } while (glitch >= (3124 >> 8));
            ICR3 = 3124;                        // 320 Hz
            Delay_ms(25);                       // delay for 25 ms

            do {
                glitch = TCNT3L;                // protect glitch
                glitch = TCNT3H;
            } while (glitch >= (2082 >> 8));
            ICR3 = 2082;                        // 480 Hz
            Delay_ms(25);                       // delay for 25 ms
        }

        TCCR3B &= ~_BV(CS32);                   // speaker off
        Delay_ms(1000);                         // delay for 1 sec
    }

    return 0;
}
