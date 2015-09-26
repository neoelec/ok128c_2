/* ========================================================================== */
/*                       Exp03_2.c : Time Delay Function                      */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

static void Delay_us(uint8_t time_us)
{                                               /* time delay for us */
    register uint8_t i;

    for (i = 0; i < time_us; i++) {             // 4 cycle +
        asm volatile ("push     r0");           // 2 cycle +
        asm volatile ("pop      r0");           // 2 cycle +
        asm volatile ("push     r0");           // 2 cycle +
        asm volatile ("pop      r0");           // 2 cycle +
        asm volatile ("push     r0");           // 2 cycle +
        asm volatile ("pop      r0");           // 2 cycle = 16 cycle = 1 us for 16MHz
    }
}

static void Delay_ms(uint16_t time_ms)
{                                               /* time delay for ms */
    register uint16_t i;

    for (i = 0; i < time_ms; i++) {
        Delay_us(250);
        Delay_us(250);
        Delay_us(250);
        Delay_us(250);
    }
}

int main(void)
{
    DDRB = 0xFF;                                // initialize PORTB = output
    Delay_us(100);

    while (1) {
        PORTB = _BV(PB6) | _BV(PB4);            // LED1, 3 on
        Delay_ms(500);
        PORTB = _BV(PB7) | _BV(PB5);            // LED2, 4 on
        Delay_ms(500);
    }

    return 0;
}
