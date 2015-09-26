/* ========================================================================== */
/*                   Exp20_4.c : printf() Function to USART0                  */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <stdio.h>

#include <OK128.h>

static int USART0_putchar(char c, FILE * stream);
static FILE device = FDEV_SETUP_STREAM(USART0_putchar, NULL, _FDEV_SETUP_WRITE);

static void USART0_initialize(void)
{                                               /* initialize USART0 */
    UBRR0H = 0;                                 // 19200 bps
    UBRR0L = 51;
    UCSR0A = 0x00;                              // asynchronous normal mode
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);           // Rx/Tx enable, 8 data
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);         // no parity, 1 stop, 8 data
}

static int USART0_putchar(char c, FILE * stream)
{                                               /* print a character to USART0 */
    if (c == '\n') {                            // process CR(carriage return)
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = 0x0D;
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = 0x0A;
    } else {
        loop_until_bit_is_set(UCSR0A, UDRE0);   // Tx ready ?
        UDR0 = c;                               // if yes, print
    }
    return 0;
}

int main(void)
{
    uint8_t i = 1;
    double x = 0.001;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    USART0_initialize();                        // initialize USART0

    LCD_string(0x80, "printf to USART0");       // display title
    LCD_string(0xC0, "Press KEY4-KEY1!");

    stdout = &device;                           // set USART0 to stdout

    while (1) {
        switch (Key_input()) {                  // key input
        case (0xF0 & ~_BV(PF4)):
            PORTB = _BV(PB4);                   // KEY1 ?
            printf("KEY1 is OK !  i=%3d  x=%5.3f\n", i++, x);
            x += 0.001;
            break;
        case (0xF0 & ~_BV(PF5)):
            PORTB = _BV(PB5);                   // KEY2 ?
            printf("KEY2 is OK !  i=%3d  x=%5.3f\n", i++, x);
            x += 0.001;
            break;
        case (0xF0 & ~_BV(PF6)):
            PORTB = _BV(PB6);                   // KEY3 ?
            printf("KEY3 is OK !  i=%3d  x=%5.3f\n", i++, x);
            x += 0.001;
            break;
        case (0xF0 & ~_BV(PF7)):
            PORTB = _BV(PB7);                   // KEY4 ?
            printf("KEY4 is OK !  i=%3d  x=%5.3f\n", i++, x);
            x += 0.001;
            break;
        default:
            break;
        }
    }

    return 0;
}
