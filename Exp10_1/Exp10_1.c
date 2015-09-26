/* ========================================================================== */
/*                Exp10_1.c : A/D Converter ADC3 Input for VR1                */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

static void LCD_4hex(uint16_t number)
{                                               /* display 4-digit hex number */
    uint16_t i;

    i = number >> 12;                           // 16^3
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');

    i = (number >> 8) & 0x000F;                 // 16^2
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');

    i = (number >> 4) & 0x000F;                 // 16^1
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');

    i = number & 0x000F;                        // 16^0
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');
}

static void LCD_1d1(float number)
{                                               /* floating-point number x.x */
    uint16_t i, j;

    j = (int)(number * 10. + 0.5);
    i = j / 10;                                 // 10^0
    LCD_data(i + '0');
    LCD_data('.');
    i = j % 10;                                 // 10^-1
    LCD_data(i + '0');
}

int main(void)
{
    uint8_t i;
    uint16_t sum;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "   ADC3 (VR1)   ");       // display title
    LCD_string(0xC0, " 0000H = 0.0[V] ");

    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);   // ADC enable, 500kHz
    ADMUX = _BV(REFS0) | _BV(MUX1) | _BV(MUX0); // select ADC3 with AVCC
    Delay_us(150);

    while (1) {
        sum = 0;
        for (i = 0; i < 16; i++) {              // read ADC by 16 times
            ADCSRA &= ~_BV(ADIF);               // clear ADIF
            ADCSRA |= _BV(ADSC);                // start conversion
            loop_until_bit_is_set(ADCSRA, ADIF);
            sum += ADCL + ADCH * 256;           // add A/D result 16 times
            Delay_ms(1);
        }

        sum = sum >> 4;                         // divide sum by 16

        LCD_command(0xC1);                      // display in HEX
        LCD_4hex(sum);
        LCD_command(0xC9);                      // display in voltage(X.X)
        LCD_1d1(sum * 5. / 1024.);              // Volt = sum*5/1024

        Delay_ms(200);                          // delay 200 ms
    }

    return 0;
}
