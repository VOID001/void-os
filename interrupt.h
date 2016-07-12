#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stddef.h>
#include <stdint.h>
#include "c_x86asm.h"

#define PIC0_CONTROL 0x20
#define PIC0_DATA 0x21
#define PIC1_CONTROL 0xA0
#define PIC1_DATA 0xA1

void init_pic();


#endif
