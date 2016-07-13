#include "terminal.h"
#include "vga.h"
#include "stdio.h"
#include "idt.h"
#include "interrupt.h"
#include "kbd_driver.h"

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
    uint8_t set = get_current_scanset();
    printf("Current Scanset is %x\n", set);
    
    for(;;) {
            asm("hlt");
    }
}
