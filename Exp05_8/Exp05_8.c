/* ========================================================================== */
/*            Exp05_8.c : INT7 Interrupt Count Program - Debouncing           */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <OK128.h>

static void LCD_3d(uint16_t number)
{                                               /* display 3-digit decimal number */
    uint8_t i, flag;

    flag = 0;
    i = number / 100;                           // 10^2
    if (i == 0)
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

static uint8_t int_count = 0;                   // interrupt count

ISR(INT7_vect)
{                                               /* INT7 interrupt function */
    Beep();                                     // beep and delay for debouncing
    int_count++;                                // increment interrupt count
    LCD_command(0xC6);                          // display interrupt count
    LCD_3d(int_count);

loop:
    loop_until_bit_is_clear(PINE, PE7);         // wait until SW2 off(INT7=1)
    Delay_ms(50);                               // delay for debouncing
    if (bit_is_clear(PINE, PE7))
        goto loop;

    EIFR |= _BV(INTF7);                         // clear INT7 interrupt flag
}

int main(void)
{
    uint8_t i, LED;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "   INT7 Count   ");       // display title
    LCD_string(0xC0, "      000       ");

    EICRB = _BV(ISC71);                         // INT7 = falling edge trigger
    EIMSK = _BV(INT7);                          // enable INT7
    EIFR = 0xFF;                                // clear interrupt flag
    sei();                                      // global interrupt enable

    while (1) {
        for (i = 1, LED = 0x10; i <= 4; i++) {  // shift from LED1 to LED4
            PORTB = LED;
            Delay_ms(200);
            LED <<= 1;
        }
    }

    return 0;
}
