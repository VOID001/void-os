#include "mem.h"

void memset(void *mem, uint8_t data, size_t count)
{
    for(size_t i = 0; i < count; i++)
        *((uint8_t*)mem + i) = data; 
}
