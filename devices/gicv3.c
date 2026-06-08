#include <stdint.h>
#include <devices/gicv3.h>
#include <arch/arch.h>










void gic_init(void) {
    uint32_t waker = GICR_WAKER;
    waker &= ~(1UL << 1UL); // Clear ProcessorSleep (Bit 1)
    GICR_WAKER = waker;

    while (GICR_WAKER & (1UL << 2UL)); // Wait for ChildrenAsleep (Bit 2) to clear

    GICR_IGROUPR = 0xFFFFFFFF;
    GICR_IGRPMODR = 0x00000000;

    GICR_ICENABLER0 = 0xFFFFFFFF;
    GICR_ICPENDR0 = 0xFFFFFFFF;

    uint32_t it_lines = GICR_TYPER & 0x1F;
    uint32_t num_regs = it_lines + 1;

    for (uint32_t i = 0; i < num_regs; i++) {
        GICD_IGROUPR(i) = 0xFFFFFFFF;
        GICD_IGRPMODR(i) = 0x00000000;

        GICD_ICENABLER(i) = 0xFFFFFFFF;
        GICD_ICPENDR(i) = 0xFFFFFFFF;
    }

    uint32_t ctlr = 0;
    ctlr |= GICD_CTLR_ENABLE_GRP0;
    ctlr |= GICD_CTLR_ENABLE_GRP1_S;
    ctlr |= GICD_CTLR_ARE_S;
    ctlr |= GICD_CTLR_ARE_NS;
    ctlr |= GICD_CTLR_ENABLE_GRP1_NS;
    GICD_CTLR = ctlr;

    icc_pmr_el3_write(PRIORITY_LOW);
    icc_bpr0_write(0);
    icc_bpr1_write(0);
    
    icc_igrpen0_write((1UL << 0UL));
    icc_igrpen1_write((1UL << 0UL));

    icc_ctlr_el3_write(0);
    isb();

    enable_irqs();
}

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
    uint32_t irq_id = icc_iar0_el3_read();

    if (irq_id == SPURIOUS_INTERRUPT) {
        return -1;
    }

    return irq_id;
}

void gic_end_interrupt(uint32_t irq_id) {
    icc_eoir0_el3_write(irq_id);
}