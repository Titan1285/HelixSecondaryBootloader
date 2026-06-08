#include <stdio.h>
#include <devices/uart.h>







void putc(char ch) {
    uart_putc(ch);
}