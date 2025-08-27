#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef struct
{
    volatile uint8_t *cs_port;
    volatile uint8_t *cs_ddr;
    uint8_t cs_pin;
} spi_handler_t;

extern spi_handler_t spi_handler;

void spi_init(volatile uint8_t *cs_port, volatile uint8_t *cs_ddr, uint8_t cs_pin);
void spi_send(uint8_t data);
void spi_select(void);
void spi_deselect(void);

#endif