#include <stdint.h>









void exc_handler(void) {
    while(1) __asm__ volatile("nop\n\t");
}