#include "terminal.h"
#include "vga.h"
#include "stdio.h"
#include "idt.h"
#include "interrupt.h"
#include "kbd_driver.h"

void kernel_main()
{
    /* Initialize terminal interface */
    terminal_initialize();

    init_idt();
    init_pic();
    init_io();
    init_keyboard();
    /* Accept Intterupt now */
    asm volatile("sti");
    printf("VOID-OS Operating System 0.01\n\n");
    printf("void-os$ ");
    while(1)
    {
		char cmd[12];
		scanf("%s",cmd);
		printf("sh: commmand not found: %s\n", cmd);
		printf("void-os$ ");

    }
    for(;;) {
            asm("hlt");
    }
}
