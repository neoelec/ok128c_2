/* ========================================================================== */
/*                  Exp06_1.c : Timer/Counter1 Interrupt(1Hz)                 */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <OK128.h>

ISR(TIMER1_COMPA_vect)
{                                               /* OC1A interrupt function */
    PORTB = PORTB ^ _BV(PB7);                   // toggle LED4
}

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "     Timer1     ");       // display title
    LCD_string(0xC0, " 1 Hz Interrupt ");

    PORTB = _BV(PB7);                           // LED4 on

    TCCR1A = 0x00;                              // CTC mode(4), don't output OC1A
    TCCR1B = _BV(WGM12) | _BV(CS12);            // 16MHz/256/(1+62499) = 1Hz
    TCCR1C = 0x00;
    OCR1A = 62499;
    TCNT1 = 0x0000;                             // clear Timer/Counter1

    TIMSK = _BV(OCIE1A);                        // enable OC1A interrupt
    ETIMSK = 0x00;
    TIFR = _BV(ICF1) | _BV(OCF1A) | _BV(OCF1B) | _BV(TOV1); // clear all interrupt flags
    ETIFR = _BV(OCF1C);
    sei();                                      // global interrupt enable

    while (1) ;                                 // wait interrupt

    return 0;
}
