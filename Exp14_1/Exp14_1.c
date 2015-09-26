/* ========================================================================== */
/*                  Exp14_1.c : AT24C16 Read/Write using TWI                  */
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

static uint8_t AT24C16_READ(uint16_t address)
{                                               /* read a byte */
    uint8_t byte;

    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // START condition
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != _BV(TWS3))) ;     // START complete ?

    TWDR = 0xA0 + ((address >> 7) & 0x000E);    // SLA+W
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS4) | _BV(TWS3)))) ; // SLA+W complete ?

    TWDR = (address & 0x00FF);                  // word address
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS5) | _BV(TWS3)))) ; // address complete ?

    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // REPEATED START condition
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != _BV(TWS4))) ;     // START complete ?

    TWDR = 0xA0 + ((address >> 7) & 0x000E) + 0x01; // SLA+R
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != _BV(TWS6))) ;     // SLA+R complete ?

    TWCR = _BV(TWINT) | _BV(TWEN);              // read data with no acknowledge
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS6) | _BV(TWS4) | _BV(TWS3)))) ;   // data complete ?
    byte = TWDR;                                // read data

    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // STOP condition

    return byte;                                // return data
}

static void AT24C16_WRITE(uint16_t address, uint8_t byte)
{                                               /* write a byte */
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // START condition
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != _BV(TWS3))) ;     // START complete ?

    TWDR = 0xA0 + ((address >> 7) & 0x000E);    // SLA+W
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS4) | _BV(TWS3)))) ; // SLA+W complete ?

    TWDR = (address & 0x00FF);                  // word address
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS5) | _BV(TWS3)))) ; // address complete ?

    TWDR = byte;                                // write data
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (bit_is_clear(TWCR, TWINT) ||
            ((TWSR & 0xF8) != (_BV(TWS5) | _BV(TWS3)))) ; // data complete ?

    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // STOP condition

    Delay_ms(5);                                // delay 5 ms for twr time
}

int main(void)
{
    uint8_t byte;
    uint16_t address;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, " AT24C16 EEPROM ");       // display title
    LCD_string(0xC0, "0x0000=0x00/0x00");

    TWBR = 12;                                  // 400kHz(TWBR=12, TWPS=0)
    TWSR = 0x00;

    for (address = 0x0000; address <= 0x07FF; address++) {
        LCD_command(0xC2);                      // display address
        LCD_4hex(address);

        LCD_command(0xC9);                      // display write data
        LCD_2hex(0x55);
        AT24C16_WRITE(address, 0x55);           // write 0x55
        byte = AT24C16_READ(address);
        LCD_command(0xCE);                      // display read data
        LCD_2hex(byte);
        Delay_ms(500);
        if (byte != 0x55) {                     // if error, beep
            Beep();
            Delay_ms(2000);
        }

        LCD_command(0xC9);                      // display write data
        LCD_2hex(0xAA);
        AT24C16_WRITE(address, 0xAA);           // write 0xAA
        byte = AT24C16_READ(address);
        LCD_command(0xCE);                      // display read data
        LCD_2hex(byte);
        Delay_ms(500);
        if (byte != 0xAA) {                     // if error, beep
            Beep();
            Delay_ms(2000);
        }
    }

    Beep();
    while (1) ;

    return 0;
}
