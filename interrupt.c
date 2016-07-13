#include "interrupt.h"
#include "kbd_driver.h"

void init_pic(void)
{
    uint8_t a1, a2;

    uint8_t map_irsq_at = 0x20;

    /* Store the mask first */
    a1 = inb(PIC0_DATA);
    a2 = inb(PIC1_DATA);

    /* Start to init PIC */

    outb(PIC0_CONTROL, 0x11);
    outb(PIC1_CONTROL, 0x11);

    outb(PIC0_DATA, map_irsq_at);
    outb(PIC1_DATA, map_irsq_at + 8);

    outb(PIC0_DATA, 0x04);
    outb(PIC1_DATA, 0x02);

    outb(PIC0_DATA, 0x01);
    outb(PIC1_DATA, 0x01);

    /* Unmask all interrupts */
    outb(PIC0_DATA, 0xfd);
    outb(PIC1_DATA, 0xff);
}

/* Interrupt handlers */

void handler_stub(void)
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
    return ;
}

void handler_21h(void)
{
    translate_scancode();
    outb(0x20, 0x20);
}

void handler_20h(void)
{
    outb(0x20, 0x20);
}
