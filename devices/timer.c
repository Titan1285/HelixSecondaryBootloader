#include <stdio.h>
#include <stdint.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/timer.h>
#include <arch/irq.h>













void timer_init(void) {
    // Disable timer
    timer_phys_disable();

    // Setup physical timer GIC config
    gic_enable_irq(26);
    gic_set_priority(26, PRIORITY_DEFAULT);

    // Register timer IRQ handler
    irq_register(26, timer_irq_handler);
}

void timer_phys_enable(void) {
    uint64_t cntp = cnthp_ctl_el2_read();
    cntp &= ~(TIMER_CNTV_IMASK);
    cntp |= TIMER_CNTV_ENABLE;
    cnthp_ctl_el2_write(cntp);
}

void timer_phys_disable(void) {
    uint64_t cntp = cnthp_ctl_el2_read();
    cntp |= TIMER_CNTV_IMASK;
    cntp &= ~(TIMER_CNTV_ENABLE);
    cnthp_ctl_el2_write(cntp);
}

void timer_set_phys(uint64_t cval) {
    cnthp_cval_el2_write(cval);
}

void timer_irq_handler(void) {
    timer_phys_disable();

    printf("Timer IRQ handler called!\n");

    gic_clear_pending(26);
}