#include <stdio.h>
#include <devices/uart.h>









void puts(const char *s) {
    uart_puts(s);
}