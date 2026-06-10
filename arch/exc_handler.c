#include <stdio.h>
#include <stdint.h>









void exc_handler(void) {
    printf("Error!\n");
    while(1) __asm__ volatile("nop\n\t");
}