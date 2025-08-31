#include "lcd.h"

void put_pixel(int x, int y, int on)
{
    ssd1306_draw_pixel(x, y, on);
}
void display()
{
    ssd1306_update();
}
void clear()
{
    ssd1306_clear();
}