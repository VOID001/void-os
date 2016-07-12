/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#include <stdbool.h> /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 
uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
void terminal_scroll();
void copy_one_line(const size_t, const size_t);
 
void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
		for (size_t x = 0; x < VGA_WIDTH; x++)
        {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c)
{
    if(c != '\n')
    {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }
    else
    {
        terminal_row++;
        terminal_column = 0;
    }
    if (terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row++;
    }
    if (terminal_row == VGA_HEIGHT)
        terminal_scroll();

}

void terminal_writestring(const char* data)
{
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        terminal_putchar(data[i]);
}

/* Scroll one line when reach the bottom of the screen */
void terminal_scroll()
{
    for(size_t i = 1; i < VGA_HEIGHT; i++)
        copy_one_line(i - 1, i);
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

void copy_one_line(const size_t dst_row, const size_t src_row)
{
    const size_t dst_offset = dst_row * VGA_WIDTH;
    const size_t src_offset = src_row * VGA_WIDTH;

    /* Copy one line (VGA_WIDTH bytes) from src to dst */

    for(size_t i = 0; i < VGA_WIDTH; i++)
        terminal_buffer[dst_offset + i] = terminal_buffer[src_offset + i];
}

void terminal_writenumber(uint32_t num, int base)
{
    static char digits[] = "0123456789abcdef";
    char buf[20];
    int cnt = 0;

    while(num)
    {
        buf[cnt++] = digits[num % base];
        num /= base;
    }
    if(base == 16)
    {
        // It's in reversed order
        buf[cnt++] = 'x';
        buf[cnt++] = '0';
    }
    if(base == 8)
    {
        buf[cnt++] = '0';
    }

    for(int i = 0; i < cnt/2; i++)
    {
        int tmp = buf[i];
        buf[i] = buf[cnt - i - 1];
        buf[cnt - i - 1] = tmp;
    }

    buf[cnt] = 0;
    terminal_writestring(buf);
}

void kernel_main() {
    /* Initialize terminal interface */
    terminal_initialize();

    /* Since there is no support for newlines in terminal_putchar
     * yet, '\n' will produce some VGA specific character instead.
     * This is normal.
     */
    for(int i = 0; i <= 333; i++)
    {
        terminal_setcolor(make_color((enum vga_color)i, 0));
        terminal_writenumber(i, 10);
        terminal_writestring("\n");
        for(int j = 0; j < 21474830; j++) ;
    }
}
