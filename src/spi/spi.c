#include <avr/io.h>
#include "spi.h"

spi_handler_t spi_handler;

void spi_init(volatile uint8_t *cs_port, volatile uint8_t *cs_ddr, uint8_t cs_pin)
{
    DDRB |= (1 << PB3) | (1 << PB5);
    DDRB &= ~(1 << PB4);

    DDRB |= (1 << PB2);

    *cs_ddr |= (1 << cs_pin);
    *cs_port |= (1 << cs_pin);

    spi_handler.cs_port = cs_port;
    spi_handler.cs_ddr = cs_ddr;
    spi_handler.cs_pin = cs_pin;

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

void spi_select(void)
{
    *spi_handler.cs_port &= ~(1 << spi_handler.cs_pin);
}

void spi_deselect(void)
{
    *spi_handler.cs_port |= (1 << spi_handler.cs_pin);
}
