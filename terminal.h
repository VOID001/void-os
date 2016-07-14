#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "vga.h"

void terminal_initialize();
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_scroll();
void copy_one_line(const size_t dst_row, const size_t src_row);
void terminal_writenumber(uint32_t num, int base);
size_t strlen(const char* str);
void terminal_clear_screen();
void terminal_set_cursor(size_t, size_t);
#endif

