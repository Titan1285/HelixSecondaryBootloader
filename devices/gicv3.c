#include <stdio.h>
#include <stdint.h>
#include <devices/gicv3.h>
#include <arch/arch.h>













void gic_enable_irq(uint32_t irq_id) {
    if (irq_id < SPI_INTD_MIN) {
        GICR_ISENABLER0 |= (1UL << irq_id);
    } else {
        uint32_t idx = irq_id / 32;
        uint64_t bit = irq_id % 32;

        GICD_ISENABLER(idx) |= (1UL << bit);
    }
}

void gic_clear_pending(uint32_t irq_id) {
    if (irq_id < SPI_INTD_MIN) {
        GICR_ICPENDR0 |= (1UL << irq_id);
    } else {
        uint32_t idx = irq_id / 32;
        uint32_t bit = irq_id % 32;

        GICD_ICPENDR(idx) |= (1UL << bit);
    }
}

void gic_set_priority(uint32_t irq_id, uint8_t prio) {
    if (irq_id < SPI_INTD_MIN) {
        GICR_IPRIORITYR(irq_id) = prio;
    } else {
        GICD_IPRIORITYR(irq_id) = prio;
    }
}

void gic_disable_irq(uint32_t irq_id) {
    if (irq_id < SPI_INTD_MIN) {
        GICR_ICENABLER0 |= (1UL << irq_id);
    } else {
        uint32_t idx = irq_id / 32;
        uint32_t bit = irq_id % 32;

        GICD_ICENABLER(idx) |= (1UL << bit);
    }
}

uint32_t gic_ack_interrupt(void) {
    uint32_t irq_id = icc_iar1_el1_read();

    if (irq_id == SPURIOUS_INTERRUPT) {
        printf("GIC: Spurious interrupt!\n");
        return -1;
    }

    return irq_id;
}

void gic_end_interrupt(uint32_t irq_id) {
    icc_eoir1_el1_write(irq_id);
}