/* ========================================================================== */
/*                Exp11_5.c : Variable D/A Voltage by Key Input               */
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

static void Dsp_voltage(uint8_t number)
{                                               /* display D/A voltage */
    uint8_t i;
    uint16_t j;

    LCD_command(0xC2);                          // display in HEX
    LCD_2hex(number);

    LCD_command(0xC7);                          // display sign of voltage
    if (number >= 0x80) {                       //   and calculate absolute value
        LCD_data('+');
        i = number - 0x80;
    } else {
        LCD_data('-');
        i = 0x80 - number;
    }

    j = i * 100 / 128;                          // voltage = (absolute/128)*10
    i = j / 100;
    if (i == 0)
        LCD_data(' ');                          // 10^1
    else
        LCD_data(i + '0');
    i = j % 100;
    LCD_data(i / 10 + '0');                     // 10^0
    LCD_data('.');
    LCD_data(i % 10 + '0');                     // 10^-1
}

int main(void)
{
    uint8_t voltage;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module
    Beep();

    LCD_string(0x80, "   D/A Output   ");       // display title
    LCD_string(0xC0, "0x80 = + 0.0 [V]");

    PORTB = _BV(PB7);                           // initial D/A output value
    voltage = 0x80;

    while (1) {
        switch (Key_input()) {                  // key input
        case (0xF0 & ~_BV(PF4)):
            voltage--;                          // if KEY1, decrement by 0x01
            PORTB = voltage;
            Dsp_voltage(voltage);
            break;
        case (0xF0 & ~_BV(PF5)):
            voltage++;                          // if KEY2, increment by 0x01
            PORTB = voltage;
            Dsp_voltage(voltage);
            break;
        case (0xF0 & ~_BV(PF6)):
            voltage -= 0x10;                    // if KEY3, decrement by 0x10
            PORTB = voltage;
            Dsp_voltage(voltage);
            break;
        case (0xF0 & ~_BV(PF7)):
            voltage += 0x10;                    // if KEY4, increment by 0x10
            PORTB = voltage;
            Dsp_voltage(voltage);
            break;
        default:
            break;
        }
    }

    return 0;
}
