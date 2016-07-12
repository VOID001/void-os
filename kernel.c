#include "terminal.h"
#include "vga.h"
#include "stdio.h"
#include "idt.h"
#include "interrupt.h"

void kernel_main() {
    /* Initialize terminal interface */
    terminal_initialize();

    /* Since there is no support for newlines in terminal_putchar
     * yet, '\n' will produce some VGA specific character instead.
     * This is normal.
     */
    int a = 233;
    uint32_t deadbeef = 0xdeadbeef;
    printf("Hello world %d %x\n", a, deadbeef);
    printf("Hello world %d %x\n", a, deadbeef);
    init_idt();
    init_pic();
    asm volatile("sti");
    for(;;) {
            asm("hlt");
    }

    //for(int i = 0; i <= 333; i++)
    //{
    //    terminal_setcolor(make_color((enum vga_color)i, 0));
    //    terminal_writenumber(i, 10);
    //    terminal_writestring("\n");
    //    for(int j = 0; j < 21474830; j++) ;
    //}
}
