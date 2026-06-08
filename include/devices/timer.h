#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>



#define TIMER_PHYS_IRQ      30
#define TIMER_VIRT_IRQ      27
#define TIMER_SEC_PHYS_IRQ  29

// NOTE: Can be used with CNTP register too...
#define TIMER_CNTV_ISTATUS  (1UL << 2UL)
#define TIMER_CNTV_IMASK    (1UL << 1UL)
#define TIMER_CNTV_ENABLE   (1UL << 0UL)





typedef struct {
    uint64_t    freq;
    uint64_t    ticks;
} time_t;










void timer_init(void);
void timer_irq_handler(void);
void timer_phys_enable(void);
void timer_phys_disable(void);
void timer_virt_disable(void);
void timer_virt_enable(void);
uint64_t timer_get_freq(void);
uint64_t timer_phys_now(void);
uint64_t timer_virt_now(void);
void timer_set_phys(uint32_t cval);
void timer_set_virt(uint32_t cval);

#endif