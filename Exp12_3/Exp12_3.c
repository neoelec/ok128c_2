/* ========================================================================== */
/*          Exp12_3.c : RS-232C Communication using USART0 Interrupt          */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <OK128.h>

static uint8_t cursor;

ISR(USART0_RX_vect)
{                                               /* UART0 interrupt function */
    LCD_data(UDR0);                             // display a character
    cursor++;                                   // 16 character OK ?
    if (cursor == 16) {
        LCD_command(0xC0);                      // if yes, go home
        cursor = 0;
        Beep();
    }
}

static void TX0_char(char data)
{                                               /* transmit a character by USART0 */
    loop_until_bit_is_set(UCSR0A, UDRE0);       // data register empty ?
    UDR0 = data;
}

static void TX0_string(char *string)
{                                               /* transmit a string by USART0 */
    while (*string != '\0') {
        TX0_char(*string);
        string++;
    }
}

int main(void)
{
    uint8_t RXD;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    UBRR0H = 0;                                 // 19200 bps
    UBRR0L = 51;
    UCSR0A = 0x00;                              // asynchronous normal mode
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0); // interrupt enable, 8 data
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);         // no parity, 1 stop, 8 data
    RXD = UDR0;                                 // dummy read
    sei();                                      // global interrupt enable

    LCD_string(0x80, "RS-232C (USART0)");       // display title
    LCD_string(0xC0, "                ");

    LCD_command(0x0F);                          // cursor ON
    LCD_command(0xC0);                          // initial cursor position
    cursor = 0;

    while (1) {
        switch (Key_input()) {                  // key input
        case (0xF0 & ~_BV(PF4)):
            PORTB = _BV(PB4);                   // KEY1 ?
            TX0_string("  KEY1 is OK !  ");
            TX0_char(0x0D);
            TX0_char(0x0A);
            break;
        case (0xF0 & ~_BV(PF5)):
            PORTB = _BV(PB5);                   // KEY2 ?
            TX0_string("  KEY2 is OK !  ");
            TX0_char(0x0D);
            TX0_char(0x0A);
            break;
        case (0xF0 & ~_BV(PF6)):
            PORTB = _BV(PB6);                   // KEY3 ?
            TX0_string("  KEY3 is OK !  ");
            TX0_char(0x0D);
            TX0_char(0x0A);
            break;
        case (0xF0 & ~_BV(PF7)):
            PORTB = _BV(PB7);                   // KEY4 ?
            TX0_string("  KEY4 is OK !  ");
            TX0_char(0x0D);
            TX0_char(0x0A);
            break;
        default:
            break;
        }
    }

    return 0;
}
