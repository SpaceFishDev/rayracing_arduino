#ifndef VECTOR_H
#define VECTOR_H
#include <stdint.h>

extern uint8_t memory[512];

void *get_mem(uint32_t size);

template <class t>
class vector
{
public:
    t *mem_addr;
    int max_num;
    int num = 0;
    vector(int max_num = 10)
    {
        mem_addr = get_mem(max_num * sizeof(t));
        this->max_num = max_num;
    }
    void add(t val)
    {
        if (num + 1 > max_num)
        {
            return;
        }
        mem_addr[num] = val;
        num += 1;
    }
    t operator[](int idx)
    {
        if (idx >= max_num)
        {
            return mem_addr[0];
        }
        return mem_addr[idx];
    }
    t get(int idx)
    {
        return (*this)[idx];
    }
};

#endif