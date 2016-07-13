#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdbool.h>
#include "terminal.h"

#define MAX_BUF_SIZE 5120

char bufio[MAX_BUF_SIZE];
int bufio_current_size;
int bufio_head_cursor;
int bufio_tail_cursor;

int printf(const char *fmt, ...);
int scanf(const char *fmt, ...);

void write_to_bufio(const char);
char read_from_bufio(void);
void flush_buf(void);
void init_io(void);
bool bufio_is_full(void);
bool bufio_is_empty(void);
char getchar(void);

#endif
