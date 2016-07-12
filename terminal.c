#include "terminal.h"

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

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
