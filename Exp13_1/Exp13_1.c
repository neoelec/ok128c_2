/* ========================================================================== */
/*                  Exp13_1.c : AT93C56 Read/Write using SPI                  */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>

static void LCD_2hex(uint8_t number)
{                                               /* display 2-digit hex number */
    uint8_t i;

    i = (number >> 4) & 0x0F;                   // 16^1
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');

    i = number & 0x0F;                          // 16^0
    if (i <= 9)
        LCD_data(i + '0');
    else
        LCD_data(i - 10 + 'A');
}

static void AT93C56_EWEN(void)
{                                               /* write enable */
    PORTB = _BV(PB0);                           // CS = 1
    SPDR = 0x09;                                // 0000 1001
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = 0x80;                                // 1000 0000
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    PORTB = 0x00;                               // CS = 0
}

static void AT93C56_EWDS(void)
{                                               /* write disable */
    PORTB = _BV(PB0);                           // CS = 1
    SPDR = 0x08;                                // 0000 1000
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = 0x00;                                // 0000 0000
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    PORTB = 0x00;                               // CS = 0
}

static uint8_t AT93C56_READ(uint8_t address)
{                                               /* read a byte */
    uint8_t upper, lower;

    PORTB = _BV(PB0);                           // CS = 1
    SPDR = 0x0C;                                // 0000 1100
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = address;                             // A7 - A0
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = 0x00;                                // dummy 0000 0000
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    upper = SPDR;                               // read upper byte
    SPDR = 0x00;                                // dummy 0000 0000
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    lower = SPDR;                               // read lower byte
    PORTB = 0x00;                               // CS = 0

    return (upper << 1) + (lower >> 7);         // get data and return
}

static void AT93C56_WRITE(uint8_t address, uint8_t byte)
{                                               /* write a byte */
    PORTB = _BV(PB0);                           // CS = 1
    SPDR = 0x0A;                                // 0000 1010
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = address;                             // A7 - A0
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    SPDR = byte;                                // D7 - D0
    loop_until_bit_is_set(SPSR, SPIF);          // transmit complete ?
    PORTB = 0x00;                               // CS = 0

    Delay_ms(5);                                // delay 5 ms for twp time
}

int main(void)
{
    uint8_t byte;
    uint16_t address;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, " AT93C56 EEPROM ");       // display title
    LCD_string(0xC0, "0x00 = 0x00/0x00");

    DDRB = _BV(PB2) | _BV(PB1) | _BV(PB0);      // MISO=input, etc.=output
    PORTB = 0x00;                               // CS = 0
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);    // SPI mode 0, 1MHz
    SPSR = 0x00;                                // SPI2X=0

    AT93C56_EWEN();                             // AT93C56 write enable

    for (address = 0x00; address <= 0xFF; address++) {
        LCD_command(0xC2);                      // display address
        LCD_2hex(address);

        LCD_command(0xC9);                      // display write data
        LCD_2hex(0x55);
        AT93C56_WRITE(address, 0x55);           // write 0x55
        byte = AT93C56_READ(address);
        LCD_command(0xCE);                      // display read data
        LCD_2hex(byte);
        Delay_ms(500);
        if (byte != 0x55) {                     // if error, beep
            Beep();
            Delay_ms(2000);
        }

        LCD_command(0xC9);                      // display write data
        LCD_2hex(0xAA);
        AT93C56_WRITE(address, 0xAA);           // write 0xAA
        byte = AT93C56_READ(address);
        LCD_command(0xCE);                      // display read data
        LCD_2hex(byte);
        Delay_ms(500);
        if (byte != 0xAA) {                     // if error, beep
            Beep();
            Delay_ms(2000);
        }
    }

    AT93C56_EWDS();                             // AT93C56 write disable
    Beep();
    while (1) ;

    return 0;
}
