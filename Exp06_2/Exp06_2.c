/* ========================================================================== */
/*                 Exp06_2.c : Timer/Counter0 Interrupt(250Hz)                */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <OK128.h>

static volatile uint8_t interrupt_count;        // interrupt counter

ISR(TIMER0_COMP_vect)
{                                               /* OC0 interrupt function */
    PORTB = PORTB ^ _BV(PB7);                   // toggle LED4
    interrupt_count--;
    if (interrupt_count == 0) {                 // 250th interrupt ?
        interrupt_count = 250;                  // if yes, toggle LED3
        PORTB = PORTB ^ _BV(PB6);
    }
}

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "     Timer0     ");       // display title
    LCD_string(0xC0, "250 Hz Interrupt");

    PORTB = _BV(PB7) | _BV(PB6);                // LED4 and 3 on

    TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS01); // CTC mode(2), don't output OC0
    ASSR = 0x00;                                // use internal clock
    OCR0 = 249;                                 // 16MHz/256/(1+249) = 250Hz
    TCNT0 = 0x00;                               // clear Timer/Counter0

    TIMSK = _BV(OCIE0);                         // enable OC0 interrupt
    TIFR = 0x00;                                // clear all interrupt flags
    sei();                                      // global interrupt enable

    interrupt_count = 250;                      // interrupt counter

    while (1) ;                                 // wait interrupt

    return 0;
}
