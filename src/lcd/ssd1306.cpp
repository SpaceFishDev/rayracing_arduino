#include <avr/io.h>
#include <util/delay.h>
#include "../spi/spi.h"
#include "ssd1306.h"

uint8_t framebuffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

ssd1306_handler_t ssd1306_handler;
static void ssd1306_command(uint8_t cmd)
{
    *(ssd1306_handler.dc_port) &= ~(1 << ssd1306_handler.dc_pin);

    spi_select();
    spi_send(cmd);
    spi_deselect();
}
static void ssd1306_data(uint8_t data)
{
    *(ssd1306_handler.dc_port) |= (1 << ssd1306_handler.dc_pin);

    spi_select();
    spi_send(data);
    spi_deselect();
}

void ssd1306_init(volatile uint8_t *dc_port, volatile uint8_t *dc_ddr, uint8_t dc_pin,
                  volatile uint8_t *rst_port, volatile uint8_t *rst_ddr, uint8_t rst_pin)
{
    ssd1306_handler.dc_port = dc_port;
    ssd1306_handler.dc_ddr = dc_ddr;
    ssd1306_handler.dc_pin = dc_pin;
    ssd1306_handler.rst_port = rst_port;
    ssd1306_handler.rst_ddr = rst_ddr;
    ssd1306_handler.rst_pin = rst_pin;
    *(ssd1306_handler.dc_ddr) |= (1 << ssd1306_handler.dc_pin);
    *(ssd1306_handler.rst_ddr) |= (1 << ssd1306_handler.rst_pin);
    *(ssd1306_handler.rst_port) &= ~(1 << ssd1306_handler.rst_pin);
    _delay_ms(10);
    *(ssd1306_handler.rst_port) |= (1 << ssd1306_handler.rst_pin);
    ssd1306_command(0xAE); // display off
    ssd1306_command(0xD5);
    ssd1306_command(0x80);
    ssd1306_command(0xA8);
    ssd1306_command(0x3F);
    ssd1306_command(0xD3);
    ssd1306_command(0x00);
    ssd1306_command(0x40); // start line
    ssd1306_command(0x8D);
    ssd1306_command(0x14); // charge pump
    ssd1306_command(0x20);
    ssd1306_command(0x00); // horizontal addressing
    ssd1306_command(0xA1); // segment remap
    ssd1306_command(0xC8); // COM scan
    ssd1306_command(0xDA);
    ssd1306_command(0x12);
    ssd1306_command(0x81);
    ssd1306_command(0xCF); // contrast
    ssd1306_command(0xD9);
    ssd1306_command(0xF1); // precharge
    ssd1306_command(0xDB);
    ssd1306_command(0x40); // VCOM detect
    ssd1306_command(0xA4); // resume RAM
    ssd1306_command(0xA6); // normal display
    ssd1306_command(0xAF); // display on

    ssd1306_clear();
    ssd1306_update();
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
        return;

    uint16_t index = x + (y / 8) * SSD1306_WIDTH;
    if (color)
        framebuffer[index] |= (1 << (y % 8));
    else
        framebuffer[index] &= ~(1 << (y % 8));
}
void ssd1306_update(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        ssd1306_command(0xB0 + page);
        ssd1306_command(0x00);
        ssd1306_command(0x10);

        *(ssd1306_handler.dc_port) |= (1 << ssd1306_handler.dc_pin);
        spi_select();
        for (uint8_t col = 0; col < SSD1306_WIDTH; col++)
            spi_send(framebuffer[page * SSD1306_WIDTH + col]);
        spi_deselect();
    }
}
void ssd1306_clear(void)
{
    for (uint16_t i = 0; i < sizeof(framebuffer); i++)
        framebuffer[i] = 0x00;
}