/* ========================================================================== */
/*                Exp10_7.c : Analog Comparator - VR1 vs 1.23V                */
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

    LCD_string(0x80, "  Analog Comp.  ");       // display title
    LCD_string(0xC0, "  VR1 < 1.23V   ");

    ACSR = _BV(ACBG) | _BV(ACIS1) | _BV(ACIS0); // +input = 1.23V
    SFIOR |= _BV(ACME);                         // use ADC input pin
    ADCSRA &= ~_BV(ADEN);                       // ADEN = 0
    ADMUX = _BV(MUX1) | _BV(MUX0);              // -input = ADC3

    while (1) {
        if ((ACSR & 0x20) == 0x20) {            // if ACO=1, ADC3 < 1.23V
            PORTB = _BV(PB4);                   //   LED1 on
            LCD_command(0xC6);                  //   display "<"
            LCD_data('<');
        } else {                                // if ACO=0, ADC3 > 1.23V
            PORTB = _BV(PB7);                   //   LED4 on
            LCD_command(0xC6);                  //   display ">"
            LCD_data('>');
        }

        Delay_ms(100);                          // delay 100 ms
    }

    return 0;
}
