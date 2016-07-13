#include "stdio.h"
#include "mem.h"

void print_buf(void);
static inline int abs(int c)
{
    return c < 0 ? -c : c;
}

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
            case 'c':
                terminal_putchar(va_arg(ap, int));
                break;
        }
    }
    return 0;
}

int scanf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    while(*fmt)
    {
        
    }
}

/* Implement a buffer IO for future support of scanf getchar printf ... */

void init_io(void)
{
    memset(bufio, 0, sizeof(bufio));
    bufio_head_cursor = 0;
    bufio_tail_cursor = 0;
    bufio_current_size = 0;
}

void write_to_bufio(const char ch)
{
    if(bufio_is_full())
        return ;
    bufio[bufio_tail_cursor] = ch;
    bufio_tail_cursor = (bufio_tail_cursor - 1 + MAX_BUF_SIZE) % MAX_BUF_SIZE;
    bufio_current_size++;
    //print_buf();
}

char read_from_bufio(void)
{
    char ret;
    if(bufio_is_empty())
        return 0;

    ret = bufio[bufio_head_cursor];
    bufio_head_cursor = (bufio_head_cursor - 1 + MAX_BUF_SIZE) % MAX_BUF_SIZE;
    bufio_current_size--;
    return ret;
}

void flush_bufio(void)
{
    bufio_tail_cursor = 0;
    bufio_head_cursor = bufio_tail_cursor;
    bufio_current_size = 0;
}

bool bufio_is_empty(void)
{
    if(bufio_current_size == 0)
    {
        return true;
    }
    return false;
}

bool bufio_is_full(void)
{
    if(bufio_current_size == MAX_BUF_SIZE)
    {
        printf("LIBO IS FULL!\n");
        return true;
    }
    return false;
}

char getchar(void)
{
    while(bufio_is_empty());

    int cursor = bufio_head_cursor;

    while(1)
    {
        cursor = (cursor - 1 + MAX_BUF_SIZE) % MAX_BUF_SIZE;
        if(cursor == bufio_tail_cursor)
            cursor = bufio_head_cursor;
        //printf("Char read from bufio %c\n", bufio[cursor]);
        if(bufio[cursor] == '\n' && !bufio_is_empty())
            break;
    }

    return read_from_bufio();
}

static inline int bufio_get_size(void)
{
    return bufio_current_size;
}

void print_buf(void)
{
    printf("\n");
    printf("Bufio tail is %d\n", bufio_tail_cursor);
    printf("Bufio head is %d\n", bufio_head_cursor);
    printf("Bufio size is %d\n", bufio_get_size());
    int cur = bufio_head_cursor;
    while(bufio_tail_cursor != cur)
    {
        printf("%c<-", bufio[cur]);
        cur = (cur - 1 + MAX_BUF_SIZE) % MAX_BUF_SIZE;
    }
    printf("\n");
}
