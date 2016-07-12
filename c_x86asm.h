/* X86 Assemble Instructions wrapper in C */
#ifndef _C_X86ASM_H
#define _C_X86ASM_H
#include <stddef.h>
#include <stdint.h>

// Read the data from IO port
static inline uint8_t inb(uint16_t port)
{
    uint8_t data;

    asm volatile ("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

// Send the data to IO port
static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile ("out %0,%1" : /* output regs empty */ :"a" (data), "d" (port));
}

#endif
