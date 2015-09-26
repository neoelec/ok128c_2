/* ========================================================================== */
/*                           Exp03_1.c : Bit Access                           */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit

    while (1) {
        PORTB = (1 << PB6) | (1 << PB4);        // LED1, 3 on
        Delay_ms(500);
        PORTB = _BV(PB7) | _BV(PB5);            // LED2, 4 on
        Delay_ms(500);
    }

    return 0;
}
