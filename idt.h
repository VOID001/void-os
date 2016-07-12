#ifndef _IDT_H
#define _IDT_H

#include <stddef.h>
#include <stdint.h>

#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_DPL_SHIFTS 5
#define IDT_FLAG_TYPE_SHIFTS 0

#define IDT_TYPE_INTERRUPT 0xE

struct gatedesc {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} ;

struct gatedesc IDT[256];


void lidt(uintptr_t base, size_t limit);
void init_idt();
void handler_stub(void);
void handler_21h(void);
void init_gatedesc_to_null(struct gatedesc *entry);
void set_gatedesc(struct gatedesc* entry, void(*handler)(void),
                uint8_t type, uint8_t dpl);

#endif
