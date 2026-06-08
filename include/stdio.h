#ifndef KSTDIO_H
#define KSTDIO_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>







void putc(char ch);
void puts(const char *s);
char getch(void);
int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list args);

// TODO: Make TinyUBSAN use printf()...
int print(const char *fmt, ...);

#endif