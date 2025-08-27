#include <avr/io.h>
#include <util/delay.h>
#include "spi/spi.h"
#include "lcd/lcd.h"
#include "text/font.h"
#include "text/text_pixels.h"

int main(void)
{
    spi_init(&PORTD, &DDRD, PD7);
    ssd1306_init(&PORTB, &DDRB, PB0,
                 &PORTB, &DDRB, PB1);
    int x = 0;
    int y = 0;
    for (int i = 0; i < 128 * 64; ++i)
    {
        if ((x + 1) >= 128)
        {
            y += 6;
            x = 0;
        }
        else
        {
            x += 4;
        }
        float r = (float)x / 128.0f;
        r *= 255.0f;
        int r_i = (int)r;
        int g = 100;
        int b_i = (int)r;
        put_txt_pixel(x, y, r_i, g, b_i);
    }
    display();
    while (1)
    {
        _delay_ms(10);
    }
    return 0;
}
