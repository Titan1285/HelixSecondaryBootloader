#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>







void irq_register(uint32_t irq_id, void (*irq_handler_func)(void));

#endif