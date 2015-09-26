#ifndef __GLCD128_H__
#define __GLCD128_H__

extern uint8_t cursor_flag;

void GLCD_command(uint8_t signal, uint8_t command);

void GLCD_data(uint8_t signal, char character);

void GLCD_clear(void);

void GLCD_xy(uint8_t x, uint8_t y);

void GLCD_xy_row(uint8_t x, uint8_t y, uint8_t row);

void GLCD_English(uint8_t Ecode);

void GLCD_Korean(uint16_t Kcode);

uint16_t KS_code_conversion(uint16_t KSSM);

void GLCD_string(uint8_t x, uint8_t y, char *string);

#endif /* __GLCD_128_H__ */
