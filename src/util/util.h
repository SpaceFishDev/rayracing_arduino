#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <avr/io.h>

extern volatile uint8_t *PORTD_PTR;
typedef enum
{
    LOW = 0,
    HIGH = 1
} pin_state;
void write_pin(volatile uint8_t *port, uint8_t pin, pin_state state);

#endif