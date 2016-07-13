#include "string.h"

char toupper(const char ch)
{
    if(ch >= 'a' && ch <='z')
        return ch - ('a' - 'A');
    return ch;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
