#include <stdint.h>
#include <devices/uart.h>



// Minimal BootROM UART





void uart_init(void) {
}

void uart_putc(char ch) {
    while(UART_FR & UART_TXFF);
    UART_DR = ch;
}

void uart_puts(const char *s) {
    while(*s != '\0') {
        uart_putc(*s);
        s++;
    }
}