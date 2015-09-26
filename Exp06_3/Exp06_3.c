/* ========================================================================== */
/*                 Exp06_3.c : Timer/Counter1 and 3 Interrupt                 */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <OK128.h>

static volatile uint16_t OC1A_count, OC3A_count;    // interrupt counter

static void LCD_5d(uint16_t number)
{                                               /* display 5-digit decimal number */
    uint16_t i;
    uint8_t flag;

    flag = 0;
    i = number / 10000;                         // 10^4
    if (i == 0)
        LCD_data(' ');
    else {
        LCD_data(i + '0');
        flag = 1;
    }

    number = number % 10000;                    // 10^3
    i = number / 1000;
    if ((i == 0) && (flag == 0))
        LCD_data(' ');
    else {
        LCD_data(i + '0');
        flag = 1;
    }

    number = number % 1000;                     // 10^2
    i = number / 100;
    if ((i == 0) && (flag == 0))
        LCD_data(' ');
    else {
        LCD_data(i + '0');
        flag = 1;
    }

    number = number % 100;                      // 10^1
    i = number / 10;
    if ((i == 0) && (flag == 0))
        LCD_data(' ');
    else {
        LCD_data(i + '0');
        flag = 1;
    }

    i = number % 10;                            // 10^0
    LCD_data(i + '0');
}

ISR(TIMER1_COMPA_vect)
{                                               /* OC1A interrupt function */
    PORTB = PORTB ^ _BV(PB7);                   // toggle LED4
    OC1A_count++;                               // increment OC1A interrupt count
}

ISR(TIMER3_COMPA_vect)
{                                               /* OC3A interrupt function */
    PORTB = PORTB ^ _BV(PB6);                   // toggle LED3
    OC3A_count++;                               // increment OC3A interrupt count
}

int main(void)
{
    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, " Timer1 = 00000 ");       // display title
    LCD_string(0xC0, " Timer3 = 00000 ");

    PORTB = _BV(PB7) | _BV(PB6);                // LED4 and 3 on

    TCCR1A = 0x00;                              // CTC mode(4), don't output OC1A
    TCCR1B = _BV(WGM12) | _BV(CS12);            // 16MHz/256/(1+62499) = 1Hz
    TCCR1C = 0x00;
    OCR1A = 62499;
    TCNT1 = 0x0000;                             // clear Timer/Counter1

    TCCR3A = 0x00;                              // CTC mode(4), don't output OC3A
    TCCR3B = _BV(WGM32) | _BV(CS32);            // 16MHz/256/(1+12499) = 5Hz
    TCCR3C = 0x00;
    OCR3A = 12499;
    TCNT3 = 0x0000;                             // clear Timer/Counter3

    TIMSK = _BV(OCIE1A);                        // enable OC1A interrupt
    ETIMSK = _BV(OCIE3A);                       // enable OC3A interrupt
    TIFR = _BV(ICF1) | _BV(OCF1A) | _BV(OCF1B) | _BV(TOV1); // clear all interrupt flags
    ETIFR = _BV(ICF3) | _BV(OCF3A) | _BV(OCF3B) | _BV(TOV3) | _BV(OCF3C) | _BV(OCF1C);
    sei();                                      // global interrupt enable

    OC1A_count = 0;                             // clear OC1A interrupt counter
    OC3A_count = 0;                             // clear OC3A interrupt counter

    while (1) {                                 // wait interrupt
        LCD_command(0x8A);                      // display OC1A interrupt count
        LCD_5d(OC1A_count);
        LCD_command(0xCA);                      // display OC3A interrupt count
        LCD_5d(OC3A_count);
    }

    return 0;
}
