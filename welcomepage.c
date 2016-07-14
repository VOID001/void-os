#include "welcomepage.h"
#include "terminal.h"
#include "icon.h"

void welcomepage()
{	
	terminal_putchar('\n');
	printf("                                 LOADING SYSTEM\n");
	printf("\n");
	icon();
	for (int i = 0; i < 78; ++i)
	{
		terminal_putentryat('>', 0x0f, i, LOADHEIGHT);
		for (int j = 0; j < 50000000; ++j);
	}
	for (int temp = 0; temp < 50000000; ++temp);
	terminal_clear_screen();
	terminal_putchar('\n');
	printf("                                WELCOME TO TOY-OS");
    for (int temp = 0; temp < 5000000000; ++temp);
}
