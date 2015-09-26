/* ========================================================================== */
/*               Exp12_1.c : RS-232C Communication using USART0               */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

static char RX0_char_scan(void)
{                                               /* receive a character by USART0 */
    if (bit_is_clear(UCSR0A, RXC0))             // if data not received,
        return 0x00;                            //    return with 0x00
    else                                        // if data received,
        return UDR0;                            //    return with data
}

static char RX0_char(void)
{                                               /* receive a character by USART0 */
    loop_until_bit_is_set(UCSR0A, RXC0);        // data received ?
    return UDR0;
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
    uint8_t RXD, cursor;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    UBRR0H = 0;                                 // 19200 bps
    UBRR0L = 51;
    UCSR0A = 0x00;                              // asynchronous normal mode
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);           // Rx/Tx enable, 8 data
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);         // no parity, 1 stop, 8 data
    RXD = UDR0;                                 // dummy read

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

        RXD = RX0_char_scan();                  // receive data if any
        if (RXD != 0x00) {
            LCD_data(RXD);                      // display a character
            cursor++;                           // 16 character OK ?
            if (cursor == 16) {
                LCD_command(0xC0);              // if yes, go home
                cursor = 0;
                Beep();
            }
        }
    }

    return 0;
}
