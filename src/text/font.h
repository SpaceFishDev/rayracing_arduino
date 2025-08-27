#ifndef FONT_H
#define FONT_H

#include <stdint.h>

extern uint8_t font[128][3];

void lcd_putc(char c, int x, int y);

#endif
