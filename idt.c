#include <stdint.h>
#include <stddef.h>

#include "stdio.h"
#include "idt.h"
#include "c_x86asm.h"

/* Setup the IDT here */

void int_stub(void);
void int_21h(void);
void int_20h(void);
void print_idt_raw(int i);
void print_idt_info(int i);

void lidt(uintptr_t base, size_t limit)
{
    asm volatile ("subl $6, %%esp\n\t"
                  "movw %w0, 0(%%esp)\n\t"
                  "movl %1, 2(%%esp)\n\t"
                  "lidt (%%esp)\n\t"
                  "addl $6, %%esp" : : "rN" (limit), "r" (base));
            
}

/* Use bitwise options to setup the gate descriptors */
void set_gatedesc(struct gatedesc* entry, void(*handler)(void),
                uint8_t type, uint8_t dpl)
{
    entry->offset_low = (uintptr_t) handler >> 0 & 0xFFFF;
    entry->selector = 0x08;  // No need to modify, only kernel code segment should enter
    entry->zero = 0x00;
    entry->type_attr = IDT_FLAG_PRESENT | 
        dpl << IDT_FLAG_DPL_SHIFTS |
        type << IDT_FLAG_TYPE_SHIFTS ;
    entry->offset_high = (uintptr_t) handler >> 16 & 0xFFFF;
}

void init_gatedesc_to_null(struct gatedesc *entry)
{
    entry->offset_low = 0;
    entry->selector = 0;
    entry->zero = 0;
    entry->type_attr = 0;
    entry->offset_high = 0;
}

void init_idt()
{
    for(int i = 0; i < 256; i++)
        init_gatedesc_to_null(IDT + i);

    /* Register the handler functions */
    printf("The int_stub is at %x\n", int_stub);
    for(int i = 0; i < 32; i++)
        set_gatedesc(IDT + i, int_stub, IDT_TYPE_INTERRUPT, 0x0);

    for(int i = 32; i < 40; i++)
        set_gatedesc(IDT +i, int_20h, IDT_TYPE_INTERRUPT, 0x0);

    set_gatedesc(IDT + 0x21, int_21h, IDT_TYPE_INTERRUPT, 0x0);

    print_idt_raw(0);
    print_idt_raw(0x21);
    print_idt_info(0x21);
    print_idt_info(0x1);
    print_idt_info(0x2);

    lidt((uintptr_t)IDT, sizeof(IDT) -1);
}

void handler_stub(void)
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
    return ;
}

void handler_21h(void)
{
    printf("Congrats VOID!\n");
    char c;
    while(!(c = inb(0x60)));
    printf("Key code is %x\n", c);
    outb(0x20, 0x20);
}

void handler_20h(void)
{
    outb(0x20, 0x20);
}



void print_idt_raw(int i)
{
    printf("The No.%d IDT entry is %x %x\n", i, (uint32_t)(IDT[i].offset_high << 16 | IDT[i].type_attr), IDT[i].selector << 16 | IDT[i].offset_low);
}

void print_idt_info(int i)
{
    printf("IDT info for No.%d\n", i);
    printf("offset_low is %x\n", IDT[i].offset_low);
    printf("offset_high is %x\n", IDT[i].offset_high);
    printf("type attr is %x\n", IDT[i].type_attr);
}
