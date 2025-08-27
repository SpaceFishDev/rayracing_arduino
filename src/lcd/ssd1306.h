#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

void ssd1306_init(volatile uint8_t *dc_port, volatile uint8_t *dc_ddr, uint8_t dc_pin,
                  volatile uint8_t *rst_port, volatile uint8_t *rst_ddr, uint8_t rst_pin);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_update(void);
void ssd1306_clear(void);

typedef struct
{
    volatile uint8_t *dc_port;
    volatile uint8_t *dc_ddr;
    uint8_t dc_pin;

    volatile uint8_t *rst_port;
    volatile uint8_t *rst_ddr;
    uint8_t rst_pin;
} ssd1306_handler_t;
extern ssd1306_handler_t ssd1306_handler;
extern uint8_t framebuffer[];
#endif
