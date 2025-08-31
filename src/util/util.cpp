#include "util.h"

volatile uint8_t *PORTD_PTR = &PORTD;

void write_pin(volatile uint8_t *port, uint8_t pin, pin_state state)
{
    if (state == HIGH)
    {
        *port |= (1 << pin); // set bit
    }
    else
    {
        *port &= ~(1 << pin); // clear bit
    }
}
#include <util/delay.h>
void delay_ms(uint32_t ms)
{
    while (ms)
    {
        _delay_ms(1);
        --ms;
    }
}