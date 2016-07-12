#include "stdio.h"

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    char c, *s;
    while(*fmt)
    {
        c = *fmt++;
        if(c != '%')
        {
            terminal_putchar(c);
            continue;
        }
        c = *fmt++;
        switch(c)
        {
            case 'd':
                terminal_writenumber(va_arg(ap, int), 10);
                break;
            case 'x':
                terminal_writenumber(va_arg(ap, int), 16);
                break;
            case 'o':
                terminal_writenumber(va_arg(ap, int), 8);
                break;
            case 's':
                s = va_arg(ap, char*);
                terminal_writestring(s);
                break;
        }
    }
    return 0;
}
