#ifndef ARCH_H
#define ARCH_H

#include <stdint.h>
#include <stddef.h>






#define EC_UNKNOWN_EXCEPTION            0x00
#define EC_WFE_WFI_TRAP                 0x01
#define EC_MCR_MRC_TRAP                 0x03
#define EC_MCRR_MRRC_TRAP               0x04
#define EC_MCR_MRC_TRAP_ACCESS          0x05
#define EC_LDC_STC_TRAP                 0x06
#define EC_SME_SVE_TRAP                 0x07
#define EC_POINTER_AUTH_TRAP            0x09
#define EC_UNKNOWN_INSTRUCTION_TRAP     0x0E
#define EC_MRRC_TRAP_ACCESS             0x12
#define EC_BRANCH_TARGET_EXCEPTION      0x13
#define EC_MSRR_MRRS_TRAP               0x18
#define EC_SVC_ARM64                    0x15
#define EC_MSR_MRS_TRAP                 0x18
#define EC_SVE_TRAP                     0x19
#define EC_ERET_TRAP                    0x1A
#define EC_PAC_FAIL                     0x1C
#define EC_SME_TRAP                     0x1D
#define EC_INSTRUCTION_ABORT_LOW_EL     0x20
#define EC_INSTRUCTION_ABORT_SAME_EL    0x21
#define EC_PC_ALIGNMENT_FAULT           0x22
#define EC_DATA_ABORT_LOW_EL            0x24
#define EC_DATA_ABORT_SAME_EL           0x25
#define EC_SP_ALIGNMENT_FAULT           0x26
#define EC_MEMORY_OPERATION             0x27
#define EC_TRAP_FPU_ARM64               0x2C
#define EC_GCS                          0x2D
#define EC_ILLEGAL_TINDEX_CHANGE        0x2E
#define EC_SERROR                       0x2F
#define EC_BREAKPOINT_LOW_EL            0x30
#define EC_BREAKPOINT_SAME_EL           0x31
#define EC_SOFTWARE_STEP_LOW_EL         0x32
#define EC_SOFTWARE_STEP_SAME_EL        0x33
#define EC_BRK_ARM64                    0x3C
#define EC_PROFILING                    0x3D









#define TCR_T0SZ(x)     ((uint64_t)(x) << 0ULL)
#define TCR_T1SZ(x)     ((uint64_t)(x) << 16ULL)

#define TCR_EPD0_ENABLE    (0ULL << 7ULL)
#define TCR_EPD0_DISABLE   (1ULL << 7ULL)

#define TCR_EPD1_ENABLE    (0ULL << 23ULL)
#define TCR_EPD1_DISABLE   (1ULL << 23ULL)

#define TCR_IRGN0_NC       (0ULL << 8ULL)
#define TCR_IRGN0_WBWA     (1ULL << 8ULL)
#define TCR_IRGN0_WT       (2ULL << 8ULL)
#define TCR_IRGN0_WB       (3ULL << 8ULL)

#define TCR_ORGN0_NC       (0ULL << 10ULL)
#define TCR_ORGN0_WBWA     (1ULL << 10ULL)
#define TCR_ORGN0_WT       (2ULL << 10ULL)
#define TCR_ORGN0_WB       (3ULL << 10ULL)

#define TCR_IRGN1_NC       (0ULL << 24ULL)
#define TCR_IRGN1_WBWA     (1ULL << 24ULL)
#define TCR_IRGN1_WT       (2ULL << 24ULL)
#define TCR_IRGN1_WB       (3ULL << 24ULL)

#define TCR_ORGN1_NC       (0ULL << 26ULL)
#define TCR_ORGN1_WBWA     (1ULL << 26ULL)
#define TCR_ORGN1_WT       (2ULL << 26ULL)
#define TCR_ORGN1_WB       (3ULL << 26ULL)

#define TCR_SH0_NS         (0ULL << 12ULL)
#define TCR_SH0_OS         (2ULL << 12ULL)
#define TCR_SH0_IS         (3ULL << 12ULL)

#define TCR_SH1_NS         (0ULL << 28ULL)
#define TCR_SH1_OS         (2ULL << 28ULL)
#define TCR_SH1_IS         (3ULL << 28ULL)

#define TCR_TG0_4KB        (0ULL << 14ULL)
#define TCR_TG0_64KB       (1ULL << 14ULL)

#define TCR_TG1_4KB        (2ULL << 30ULL)
#define TCR_TG1_64KB       (3ULL << 30ULL)

#define TCR_A1_USE_TTBR0   (0ULL << 22ULL)
#define TCR_A1_USE_TTBR1   (1ULL << 22ULL)

#define TCR_IPS_32BIT      (0ULL << 32ULL)
#define TCR_IPS_36BIT      (1ULL << 32ULL)
#define TCR_IPS_40BIT      (2ULL << 32ULL)
#define TCR_IPS_48BIT      (5ULL << 32ULL)

#define TCR_ASID_8BIT      (0ULL << 36ULL)

#define TCR_TBI0_USED      (0ULL << 37ULL)
#define TCR_TBI0_IGNORED   (1ULL << 37ULL)

#define TCR_TBI1_USED      (0ULL << 38ULL)
#define TCR_TBI1_IGNORED   (1ULL << 38ULL)

#define SCTLR_ICACHE_ENABLE (1ULL << 12ULL)
#define SCTLR_DCACHE_ENABLE (1ULL << 2ULL)
#define SCTLR_MMU_ENABLE    (1ULL << 0ULL)


#define isb()           __asm__ volatile("isb" ::: "memory");
#define dsb(x)          __asm__ volatile("dsb " #x ::: "memory");
#define dmb(x)          __asm__ volatile("dmb " #x ::: "memory");
#define tlbi(x)         __asm__ volatile("tlbi " #x ::: "memory");
#define ic(x)           __asm__ volatile("ic " #x ::: "memory");
#define wfi()           __asm__ volatile("wfi" ::: "memory");







typedef struct {
    uint64_t elr_el1;
    uint64_t spsr_el1;
    uint64_t esr_el1;
    uint64_t far_el1;
    uint64_t pad; // stp x30, xzr, [sp, #-16]!
    uint64_t x30;
    uint64_t x29;
    uint64_t x28;
    uint64_t x27;
    uint64_t x26;
    uint64_t x25;
    uint64_t x24;
    uint64_t x23;
    uint64_t x22;
    uint64_t x21;
    uint64_t x20;
    uint64_t x19;
    uint64_t x18;
    uint64_t x17;
    uint64_t x16;
    uint64_t x15;
    uint64_t x14;
    uint64_t x13;
    uint64_t x12;
    uint64_t x11;
    uint64_t x10;
    uint64_t x9;
    uint64_t x8;
    uint64_t x7;
    uint64_t x6;
    uint64_t x5;
    uint64_t x4;
    uint64_t x3;
    uint64_t x2;
    uint64_t x1;
    uint64_t x0;
} __attribute__((packed, aligned(16))) cpu_ctx_t;

typedef void (*irq_handler_t)(void);
 







void exc_handler(cpu_ctx_t *ctx);
void irq_handler(void);
void irq_register(uint32_t irq_id, void (*irq_handler_func)(void));

extern void halt(void);

extern void disable_irqs(void);
extern void enable_irqs(void);

extern uint64_t get_icache_line_size(void);
extern uint64_t get_dcache_line_size(void);
extern void flush_cache_range(uint64_t base_addr, size_t length);

extern void inval_page(uint64_t pte);

extern uint64_t cnthp_ctl_el2_read(void);
extern void cnthp_ctl_el2_write(uint64_t val);
extern void cnthp_cval_el2_write(uint64_t val);

extern void icc_pmr_el1_write(uint64_t val);
extern void icc_igrpen0_write(uint64_t val);
extern void icc_igrpen1_write(uint64_t val);
extern void icc_bpr1_write(uint64_t val);
extern uint64_t icc_iar1_el1_read(void);
extern void icc_eoir1_el1_write(uint64_t val);
extern void icc_ctlr_el1_write(uint64_t val);

extern uint64_t ttbr0_el2_read(void);
extern void ttbr0_el2_write(uint64_t addr);

extern uint64_t tcr_el2_read(void);
extern void tcr_el2_write(uint64_t val);

extern uint64_t sctlr_el2_read(void);
extern void sctlr_el2_write(uint64_t val);

extern uint64_t get_icache_line_size(void);
extern uint64_t get_dcache_line_size(void);
extern void flush_cache_range(uint64_t base_addr, size_t length);

extern void arch_jump_ub(uint64_t addr);

extern uint64_t mair_el2_read(void);
extern void mair_el2_write(uint64_t val);

#endif