#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>



#define TIMER_PHYS_IRQ      26
#define TIMER_VIRT_IRQ      28

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
uint64_t timer_get_freq(void);
void timer_set_phys(uint64_t cval);

#endif