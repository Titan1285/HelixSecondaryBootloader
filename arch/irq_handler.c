#include <stdio.h>
#include <stdint.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <arch/irq.h>













static volatile irq_handler_t irq_handlers[MAX_INTD];





void irq_register(uint32_t irq_id, void (*irq_handler_func)(void)) {
    if (irq_id > MAX_INTD) {
        return;
    }

    irq_handlers[irq_id] = irq_handler_func;
}

void irq_handler(void) {
    uint32_t irq_id = gic_ack_interrupt();

    if (irq_id == 0x3FD || irq_id == 0x3FC) {
        // Received IRQ for non-secure (EL1) interrupt while in EL3
        return;
    }

    if (irq_id == SPURIOUS_INTERRUPT) {
        return;
    }

    if (irq_id > MAX_INTD) {
        return;
    }

    irq_handlers[irq_id]();

    gic_end_interrupt(irq_id);
}