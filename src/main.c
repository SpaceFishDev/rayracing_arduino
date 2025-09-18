#include <avr/io.h>
#include <util/delay.h>
#include "spi/spi.h"
#include "lcd/lcd.h"
#include "text/font.h"
#include "text/text_pixels.h"
#include "util/util.h"
#include <avr/pgmspace.h>

void blink()
{
    write_pin(&PORTD, PIN4, HIGH);
    _delay_ms(200);
    write_pin(&PORTD, PIN4, LOW);
}

extern const uint8_t img[] PROGMEM;

int main(void)
{
    spi_init(&PORTD, &DDRD, PD7);
    ssd1306_init(&PORTB, &DDRB, PB0,
                 &PORTB, &DDRB, PB1);

    sort_mappings();
    for (int i = 0; i < SSD1306_HEIGHT; ++i)
    {
        put_pixel(i, i, 1);
    }
    display();
    int x = 0;
    int y = 0;
    int sum = 0;
    int sum_idx = 0;
    int last_c = 0;
    for (int i = 0; i < 754;)
    {
        if (sum_idx == 4)
        {
            sum_idx = 0;
            float avg = sum;
            avg /= 4.0f;
            put_txt_pixel(x * 8, y * 6, (uint8_t)last_c, (uint8_t)avg, (uint8_t)avg);
            x++;
            sum = 0;
            if (x > 16)
            {
                x = 0;
                ++y;
            }
        }
        uint8_t n = pgm_read_byte(&img[i++]);
        uint8_t c = pgm_read_byte(&img[i++]);
        for (int j = 0; j < n; ++j)
        {
            sum_idx++;
            sum += c;
            if (sum_idx == 4)
            {
                sum_idx = 0;
                float avg = sum;
                avg /= 4.0f;
                put_txt_pixel(x * 8, y * 6, (uint8_t)avg, (uint8_t)avg, (uint8_t)avg);
                x++;
                sum = 0;

                if (x > 16)
                {
                    x = 0;
                    ++y;
                }
            }
        }
    }
    display();
}
