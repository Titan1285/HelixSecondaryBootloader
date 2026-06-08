#include <stdint.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/timer.h>
#include <arch/irq.h>












void timer_init(void) {
    // Disable timer
    timer_phys_disable();


    irq_register(TIMER_SEC_PHYS_IRQ, timer_irq_handler);

    // Setup physical timer GIC config
    gic_enable_irq(TIMER_SEC_PHYS_IRQ);
    gic_set_priority(TIMER_SEC_PHYS_IRQ, PRIORITY_DEFAULT);
}

void timer_phys_enable(void) {
    uint64_t cntp = cntps_ctl_el1_read();
    cntp &= ~(TIMER_CNTV_IMASK);    // Unmask IRQ's
    cntp |= TIMER_CNTV_ENABLE;      // Enable timer
    cntps_ctl_el1_write(cntp);
}

void timer_phys_disable(void) {
    uint64_t cntp = cntps_ctl_el1_read();
    cntp |= TIMER_CNTV_IMASK;       // Mask IRQ's
    cntp &= ~(TIMER_CNTV_ENABLE);   // Disable timer
    cntps_ctl_el1_write(cntp);
}

void timer_set_phys(uint32_t tval) {
    cntps_tval_el1_write(tval);
}

uint64_t timer_get_freq(void) {
    uint64_t freq = cntfrq_el0_read();

    return freq;
}

uint64_t timer_phys_now(void) {
    uint64_t ticks = cntpct_el0_read();

    return ticks;
}

uint64_t timer_virt_now(void) {
    uint64_t ticks = cntvct_el0_read();

    return ticks;
}

void timer_irq_handler(void) {
    timer_phys_disable();
    
    gic_clear_pending(TIMER_SEC_PHYS_IRQ);
}