#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <stddef.h>





#define HHDM_BASE   0xFFFF000000000000ULL

#define virt_to_phys(va) ((uint64_t)((uint64_t)(va) - HHDM_BASE))
#define phys_to_virt(pa) ((void *)((uint64_t)(pa) + HHDM_BASE))

#define PAGE_SIZE 	4096
#define PAGE_MASK	0x0000FFFFFFFFF000ULL

#define ALIGN_UP(a, b)		(((a) + (b) - 1) & ~((b) - 1))
#define ALIGN_DOWN(a, b) 	((a) & ~((b) - 1))
#define IS_ALIGNED(a, b)	(((a) & ((b) - 1)) == 0)

#define PAGE_ALIGN_UP(x) 	ALIGN_UP(x, PAGE_SIZE)
#define PAGE_ALIGN_DOWN(x)	ALIGN_DOWN(x, PAGE_SIZE)
#define IS_PAGE_ALIGNED(x)	IS_ALIGNED(x, PAGE_SIZE)

#define L0_INDEX(virt)		(((virt) >> 39) & 0x1FF)
#define L1_INDEX(virt)		(((virt) >> 30) & 0x1FF)
#define L2_INDEX(virt)		(((virt) >> 21) & 0x1FF)
#define L3_INDEX(virt)		(((virt) >> 12) & 0x1FF)

#define SIZE_TO_PAGES(x) (((x) + PAGE_SIZE - 1) / PAGE_SIZE)

#define PTE_VALID		(1ULL << 0ULL)	// Valid page
#define PTE_TABLE		(1ULL << 1ULL)	// Page table
#define PTE_PAGE		(1ULL << 1ULL)	// 4KB page
#define PTE_BLOCK		(0ULL << 1ULL)	// 2MB/1GB page
#define PTE_NS			(1ULL << 5ULL)	// Non-secure
#define PTE_RW_NA_EL0	(0ULL << 6ULL)	// Read-write EL1/EL2/EL3, no EL0 access
#define PTE_RW_ELX		(1ULL << 6ULL)	// Read-write for all exception levels
#define PTE_RO_NA_EL0	(2ULL << 6ULL)	// Read-only EL1/EL2/EL3, no EL0 access
#define PTE_RP_ELX		(3ULL << 6ULL)	// Read-only for all exception levels
#define PTE_SH_NS		(0ULL << 8ULL)	// Non-sharable
#define PTE_SH_RES		(1ULL << 8ULL)	// Don't use
#define PTE_SH_OUTER	(2ULL << 8ULL)	// Outer sharable
#define PTE_SH_INNER	(3ULL << 8ULL)	// Inner shrable
#define PTE_AF			(1ULL << 10ULL)	// Access flag
#define PTE_NG			(1ULL << 11ULL)	// Non-global
#define PTE_DBM			(1ULL << 51ULL)	// Dirty bit modfier
#define PTE_CONTIG		(1ULL << 52ULL)	// Contigous
#define PTE_PXN			(1ULL << 53ULL)	// Privilaged Access Never
#define PTE_UXN			(1ULL << 54ULL)	// User Access Never

#define PTE_ATTR_NORMAL	(0ULL << 2ULL)
#define PTE_ATTR_DEVICE (1ULL << 2ULL)

#define PTE_NORMAL_RW	    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RW_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_ATTR_NORMAL)
#define PTE_NORMAL_RO	    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RO_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_ATTR_NORMAL)
#define PTE_NORMAL_RW_NX    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RW_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_UXN | PTE_PXN | PTE_ATTR_NORMAL)
#define PTE_NORMAL_RO_NX    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RO_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_UXN | PTE_PXN | PTE_ATTR_NORMAL)

#define PTE_DEVICE_RW	    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RW_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_ATTR_DEVICE)
#define PTE_DEVICE_RO	    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RO_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_ATTR_DEVICE)

#define PTE_DEVICE_RW_NX    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RW_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_UXN | PTE_PXN | PTE_ATTR_DEVICE)
#define PTE_DEVICE_RO_NX    (PTE_VALID | PTE_PAGE | PTE_NS | PTE_RO_NA_EL0 | PTE_SH_INNER | PTE_AF | PTE_UXN | PTE_PXN | PTE_ATTR_DEVICE)

#endif