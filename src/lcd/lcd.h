#ifndef LCD_H
#define LCD_H

#include "ssd1306.h"

void put_pixel(int x, int y, int on);
void display();
void clear();

#endif