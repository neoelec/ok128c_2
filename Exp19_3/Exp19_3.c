/* ========================================================================== */
/*               Exp19_3.c : Digital Clock on Graphic LCD Module              */
/* ========================================================================== */
/*                        Designed and programmed by Duck-Yong Yoon in 2006.  */

#include <avr/io.h>

#include <OK128.h>
#include <GLCD128.h>

static void GLCD_2hex(uint8_t number)
{                                               /* display 2-digit hex number */
    uint8_t i;

    i = (number >> 4) & 0x0F;                   // 16^1
    if (i <= 9)
        GLCD_English(i + '0');
    else
        GLCD_English(i - 10 + 'A');

    i = number & 0x0F;                          // 16^0
    if (i <= 9)
        GLCD_English(i + '0');
    else
        GLCD_English(i - 10 + 'A');
}

static void GLCD_2d1(float number)
{                                               /* floating-point number xx.x */
    uint16_t i, j;

    j = (int)(number * 10. + 0.5);
    i = j / 100;                                // 10^1
    if (i == 0)
        GLCD_English(' ');
    else
        GLCD_English(i + '0');

    j = j % 100;                                // 10^0
    i = j / 10;
    GLCD_English(i + '0');
    GLCD_English('.');

    i = j % 10;                                 // 10^-1
    GLCD_English(i + '0');
}

int main(void)
{
    uint8_t i, x;
    uint16_t sum;

    MCU_initialize();                           // initialize MCU and kit
    Delay_ms(50);                               // wait for system stabilization
    LCD_initialize();                           // initialize text LCD module

    GLCD_clear();                               // initialize GLCD screen
    cursor_flag = 0;                            // cursor off

    LCD_string(0x80, "Graphic LCD Test");       // display title on text LCD
    LCD_string(0xC0, " English/Korean ");
    Beep();

    GLCD_string(0, 0, "2006년 05월 01일");      // display title
    GLCD_string(1, 0, " 오전  12:00:00 ");
    GLCD_string(2, 0, "    (월요일)    ");
    GLCD_string(3, 0, "현재기온  00.0도");

    while (1) {
        for (i = 0; i < 10; i++) {
            GLCD_xy(0, 0);                      // display year
            GLCD_2hex(0x20);
            GLCD_2hex(RTC_YEAR);
            GLCD_xy(0, 7);                      // display month
            GLCD_2hex(RTC_MONTH);
            GLCD_xy(0, 12);                     // display date
            GLCD_2hex(RTC_DATE);
            x = RTC_HOUR;                       // display AM/PM
            if (x < 0x80)
                GLCD_string(1, 3, "전");
            else
                GLCD_string(1, 3, "후");
            GLCD_xy(1, 7);                      // display hour
            GLCD_2hex(x & 0x7F);
            GLCD_xy(1, 10);                     // display minute
            GLCD_2hex(RTC_MINUTE);
            GLCD_xy(1, 13);                     // display second
            GLCD_2hex(RTC_SECOND);
            x = RTC_WEEKDAY;                    // display weekday
            if (x == 1)
                GLCD_string(2, 5, "일");
            else if (x == 2)
                GLCD_string(2, 5, "월");
            else if (x == 3)
                GLCD_string(2, 5, "화");
            else if (x == 4)
                GLCD_string(2, 5, "수");
            else if (x == 5)
                GLCD_string(2, 5, "목");
            else if (x == 6)
                GLCD_string(2, 5, "금");
            else
                GLCD_string(2, 5, "토");
            Delay_ms(100);
        }

        ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);   // ADC enable, 500kHz
        ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX0);    // select ADC1 with 2.56V
        Delay_us(150);

        sum = 0;
        for (i = 0; i < 16; i++) {              // read ADC by 16 times
            ADCSRA &= ~_BV(ADIF);               // clear ADIF
            ADCSRA |= _BV(ADSC);                // start conversion
            loop_until_bit_is_set(ADCSRA, ADIF);
            sum += ADCL + ADCH * 256;           // add A/D result 16 times
            Delay_ms(1);
        }

        sum = sum >> 4;                         // divide sum by 16

        GLCD_xy(3, 10);                         // display temperature(XX.X)
        GLCD_2d1((float)sum * 256. / 1024.);
    }

    return 0;
}
