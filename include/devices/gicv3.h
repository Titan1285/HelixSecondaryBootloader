#ifndef GICV3_H
#define GICV3_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>




// Local CPU INTD's: 0 - 15, PPI's INTD's: 16 - 31, SPI's INTD's: 32 - 1019

#define CPU_INTD_MIN    0
#define CPU_INTD_MAX    15
#define PPI_INTD_MIN    16
#define PPI_INTD_MAX    31
#define SPI_INTD_MIN    32
#define SPI_INTD_MAX    1019

#define MAX_INTD            1020
#define SPURIOUS_INTERRUPT  1023

#define PRIORITY_LOW        0xFF
#define PRIORITY_HIGH       0x00
#define PRIORITY_DEFAULT    0x80


#define GICD_BASE 0x01001000
#define GICR_BASE 0x01011000
#define GICR_SGI_BASE (GICR_BASE + 0x00010000)



#define GICD_CTLR           (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0000))
#define GICD_TYPER          (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0004))
#define GICD_IIDR           (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0008))
#define GICD_TYPER2         (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x000C))
#define GICD_STATUSR        (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0010))
#define GICD_SETSPI_NSR     (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0040))
#define GICD_CLRSPI_NSR     (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0048))
#define GICD_IGROUPR(x)     (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0080 + (x) * 4))
#define GICD_ISENABLER(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0100 + (x) * 4))
#define GICD_ICENABLER(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0180 + (x) * 4))
#define GICD_ISPENDR(x)     (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0200 + (x) * 4))
#define GICD_ICPENDR(x)     (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0280 + (x) * 4))
#define GICD_ISACTIVER(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0300 + (x) * 4))
#define GICD_ICACTIVER(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0380 + (x) * 4))
#define GICD_IPRIORITYR(x)  (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0400 + (x) * 4))
#define GICD_ITARGETSR(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0800 + (x) * 4))
#define GICD_ICFGR(x)       (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0C00 + (x) * 4))
#define GICD_IGRPMODR(x)    (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0D00 + (x) * 4))
#define GICD_NSACR(x)       (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0E00 + (x) * 4))
#define GICD_SGIR           (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0F00))
#define GICD_CPENDSGIR(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0F10 + (x) * 4))
#define GICD_SPENDSGIR(x)   (*(volatile uint32_t *)((uintptr_t)GICD_BASE + 0x0F20 + (x) * 4))

#define GICD_CTLR_DS                (1UL << 6UL)    // Disable security (allow non-secure access)
#define GICD_CTLR_ARE_S             (1UL << 4UL)    // Affinity routing enable for Secure state
#define GICD_CTLR_ARE_NS            (1UL << 5UL)    // Affinity routing enable for non-secure state
#define GICD_CTLR_ENABLE_GRP1_S     (1UL << 2UL)    // Enable group 1 interrupts for secure state
#define GICD_CTLR_ENABLE_GRP1_NS    (1UL << 1UL)    // Enable group 1 interrupts for none-secure state
#define GICD_CTLR_ENABLE_GRP0       (1UL << 0UL)    // Enable group 0 interrupts


#define GICR_CTLR           (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0000))
#define GICR_IIDR           (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0004))
#define GICR_TYPER          (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0008))
#define GICR_STATUSR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0010))
#define GICR_WAKER          (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0014))
#define GICR_MPAMIDR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0018))
#define GICR_PARTIDR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x001C))
#define GICR_SETLPIR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0040))
#define GICR_CLRLPIR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0048))
#define GICR_PROPBASER      (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0070))
#define GICR_PENDBASER      (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x0078))
#define GICR_INVLPIR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x00A0))
#define GICR_INVALLR        (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x00B0))
#define GICR_SYNCR          (*(volatile uint32_t *)((uintptr_t)GICR_BASE + 0x00C0))
#define GICR_IGROUPR        (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0080))
#define GICR_ISENABLER0     (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0100))
#define GICR_ICENABLER0     (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0180))
#define GICR_ISPENDR0       (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0200))
#define GICR_ICPENDR0       (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0280))
#define GICR_ISACTIVER0     (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0300))
#define GICR_ICACTIVER0     (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0380))
#define GICR_IPRIORITYR(x)  (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0400 + (x) * 4))
#define GICR_ICFGR0         (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0C00))
#define GICR_ICFGR1         (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0C04))
#define GICR_IGRPMODR       (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0D00))
#define GICR_NSACR          (*(volatile uint32_t *)((uintptr_t)GICR_SGI_BASE + 0x0E00))

#define GICR_CHILDREN_ASLEEP    (1UL << 2UL)
#define GICR_PROCESSOR_SLEEP    (1UL << 1UL)








void gic_init(void);
uint32_t gic_ack_interrupt(void);
void gic_end_interrupt(uint32_t irq_id);
void gic_enable_irq(uint32_t irq_id);
void gic_disable_irq(uint32_t irq_id);
void gic_clear_pending(uint32_t irq_id);
void gic_set_priority(uint32_t irq_id, uint8_t prio);

#endif