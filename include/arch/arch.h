#ifndef ARCH_H
#define ARCH_H

#include <stdint.h>





typedef void (*irq_handler_t)(void);

#define isb()           __asm__ volatile("isb" ::: "memory");
#define dsb(x)          __asm__ volatile("dsb " #x ::: "memory");
#define dmb(x)          __asm__ volatile("dmb " #x ::: "memory");
#define tlbi(x)         __asm__ volatile("tlbi " #x ::: "memory");
#define ic(x)           __asm__ volatile("ic " #x ::: "memory");
#define wfi()           __asm__ volatile("wfi" ::: "memory");


extern void icc_pmr_el3_write(uint32_t val);
extern void icc_igrpen0_write(uint32_t val);
extern void icc_igrpen1_write(uint32_t val);
extern void icc_bpr1_write(uint32_t val);
extern void icc_ctlr_el3_write(uint32_t val);
extern uint32_t icc_iar0_el3_read(void);
extern void icc_eoir0_el3_write(uint32_t val);
extern void enable_irqs(void);
extern void disable_irqs(void);

extern uint64_t cntfrq_el0_read(void);

// Physcial timers
extern uint64_t cntp_ctl_el0_read(void);
extern void cntp_ctl_el0_write(uint64_t cntp_val);
extern uint64_t cntpct_el0_read(void);
extern uint32_t cntp_cval_el0_read(void);
extern void cntp_cval_el0_write(uint32_t cntp_cval);
extern uint32_t cntp_tval_el0_read(void);
extern void cntp_tval_el0_write(uint32_t cntp_tval);

// Virtual timers
extern uint64_t cntv_ctl_el0_read(void);
extern void cntv_ctl_el0_write(uint64_t cntv_val);
extern uint64_t cntvct_el0_read(void);
extern uint32_t cntv_cval_el0_read(void);
extern void cntv_cval_el0_write(uint32_t cntv_cval);
extern uint32_t cntv_tval_el0_read(void);
extern void cntv_tval_el0_write(uint32_t cntv_tval);

extern void icc_bpr0_write(uint32_t val);
extern uint32_t cntps_ctl_el1_read(void);
extern void cntps_ctl_el1_write(uint32_t val);
extern uint32_t cntps_tval_el1_read(void);
extern void cntps_tval_el1_write(uint32_t tval);

extern void arch_jump_sbl(uint64_t base_addr);

#endif