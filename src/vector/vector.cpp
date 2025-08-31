#include "vector.h"

uint8_t memory[512];
int mem_idx = 0;
void *get_mem(uint32_t size)
{
    if (mem_idx + size > sizeof(memory))
    {
        return (void *)-1;
    }
    mem_idx += size;
    return (void *)memory[mem_idx - size];
}