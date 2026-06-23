#include <stdio.h>
#include <string.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <arch/arch.h>





static volatile uint64_t *pgd = NULL;





void vmm_init(void) {
    uint64_t tcr = 0, sctlr = 0, mair = 0;



    tcr = tcr_el2_read();
    tcr &= ~TCR_EPD0_DISABLE;
    tcr |= TCR_EPD1_DISABLE;
    tcr |= TCR_IPS_48BIT;
    tcr |= TCR_TG0_4KB;
    tcr |= TCR_ORGN0_WBWA;
    tcr |= TCR_IRGN0_WBWA;
    tcr |= TCR_SH0_IS;
    tcr |= TCR_T0SZ(16);
    tcr_el2_write(tcr);
    isb();

    mair = mair_el2_read();
    mair |= (0x00 << 0) | (0xFF << 8);
    mair_el2_write(mair);
    isb();

    pgd = vmm_page_table_alloc();
    
    if (!pgd) {
        printf("VMM: Failed to allocate L0 page table!\n");
        return;
    }

    uintptr_t rom_base = 0x00000000;
    size_t rom_size = 0x10000;

    vmm_map_page_range(rom_base, rom_base, SIZE_TO_PAGES(rom_size), PTE_NORMAL_RO);
    flush_cache_range(rom_base, rom_size);

    uintptr_t sram_base = 0x00010000;
    size_t sram_size = 0x80000;

    // TODO: Map bootloader sections instead?
    vmm_map_page_range(sram_base, sram_base, SIZE_TO_PAGES(sram_size), PTE_NORMAL_RW);
    flush_cache_range(sram_base, sram_size);

    uintptr_t mmio_base = 0x01000000;
    uintptr_t mmio_end = 0x01042000;
    size_t mmio_size = mmio_end - mmio_base;

    vmm_map_page_range(mmio_base, mmio_base, SIZE_TO_PAGES(mmio_size), PTE_DEVICE_RW_NX);
    flush_cache_range(mmio_base, mmio_size);

    uintptr_t dram_base = 0x40000000;
    size_t dram_size = 0x3FFF0000;

    vmm_map_page_range(dram_base, dram_base, SIZE_TO_PAGES(dram_size), PTE_NORMAL_RW);
    flush_cache_range(dram_size, dram_size);

    uintptr_t fb_base = 0x7FFF0000;
    size_t fb_size = 0x1000000;

    vmm_map_page_range(fb_base, fb_base, SIZE_TO_PAGES(fb_size), PTE_DEVICE_RW_NX);
    flush_cache_range(fb_base, fb_size);

    ttbr0_el2_write((uint64_t)pgd);
    isb();

    tlbi(alle2);
    dsb(sy);
    isb();

    ic(iallu);
    dsb(sy);
    isb();

    sctlr = sctlr_el2_read();
    sctlr |= SCTLR_MMU_ENABLE;
    sctlr |= SCTLR_ICACHE_ENABLE;
    sctlr |= SCTLR_DCACHE_ENABLE;
    sctlr_el2_write(sctlr);
    dsb(sy);
    isb();
}

uint64_t *vmm_page_table_alloc(void) {
    uint64_t *pt = NULL;

    pt = pmm_alloc(1);

    flush_cache_range((uint64_t)pt, PAGE_SIZE);

    if (!pt) {
        return NULL;
    }

    memset((uint64_t *)((uint64_t)pt), 0, PAGE_SIZE);

    return pt;
}

void vmm_map_page(uint64_t virt, uint64_t phys, uint64_t flags) {
    uint64_t *l0 = NULL, *l1 = NULL, *l2 = NULL, *l3 = NULL;


    l0 = (uint64_t *)((uint64_t)pgd);

    if (!(l0[L0_INDEX(virt)] & PTE_VALID)) {
        l1 = vmm_page_table_alloc();

        if (!l1) {
            printf("VMM: Failed to allocate L1 page table!\n");
            return;
        }

        l0[L0_INDEX(virt)] = ((uint64_t)l1 & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    } else {
        l1 = (uint64_t *)(l0[L0_INDEX(virt)] & PAGE_MASK);
    }

    if (!(l1[L1_INDEX(virt)] & PTE_VALID)) {
        l2 = vmm_page_table_alloc();

        if (!l2) {
            printf("VMM: Failed to allocate L2 page table!\n");
            return;
        }

        l1[L1_INDEX(virt)] = ((uint64_t)l2 & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    } else {
        l2 = (uint64_t *)(l1[L1_INDEX(virt)] & PAGE_MASK);
    }

    if (!(l2[L2_INDEX(virt)] & PTE_VALID)) {
        l3 = vmm_page_table_alloc();

        if (!l3) {
            printf("VMM: Failed to allcoate L3 page table!\n");
            return;
        }

        l2[L2_INDEX(virt)] = ((uint64_t)l3 & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    } else {
        l3 = (uint64_t *)(l2[L2_INDEX(virt)] & PAGE_MASK);
    }

    l3[L3_INDEX(virt)] = (phys & PAGE_MASK) | flags;
}

void vmm_unmap_page(uint64_t virt, uint64_t phys) {
    uint64_t *l0 = NULL, *l1 = NULL, *l2 = NULL, *l3 = NULL;
    uint64_t pte = 0;


    // TODO: Handle freeing of empty page tables, so we don't run out of allocatable memory

    l0 = (uint64_t *)((uint64_t)pgd);


    if (l0[L0_INDEX(virt)] & PTE_VALID) {
        l1 = (uint64_t *)(l0[L0_INDEX(virt)] & PAGE_MASK);
    } else {
        return;
    }

    if (l1[L1_INDEX(virt)] & PTE_VALID) {
        l2 = (uint64_t *)(l1[L1_INDEX(virt)] & PAGE_MASK);
    } else {
        return;
    }

    if (l2[L2_INDEX(virt)] & PTE_VALID) {
        l3 = (uint64_t *)(l2[L2_INDEX(virt)] & PAGE_MASK);
    } else {
        return;
    }

    // Get phys PTE
    pte = (l3[L3_INDEX(virt)] & PAGE_MASK);

    if (phys != pte) {
        printf("VMM: PTE mismatch! (Got %llx, expected %llx)\n", phys, pte);
        return;
    }

    // Invalidate this entry
    l3[L3_INDEX(virt)] = 0;

    // Invalidate pages TLB
    //inval_page(virt);
}

void vmm_map_page_range(uint64_t virt, uint64_t phys, uint64_t num_pages, uint64_t flags) {
    uint64_t virt_base = PAGE_ALIGN_DOWN(virt);
    uint64_t phys_base = PAGE_ALIGN_DOWN(phys);

    for (uint64_t i = 0; i < num_pages; i++) {
        vmm_map_page(virt_base + (i * PAGE_SIZE), phys_base + (i * PAGE_SIZE), flags);
    }
}

void vmm_unmap_page_range(uint64_t virt, uint64_t phys, uint64_t num_pages) {
    for (uint64_t i = 0; i < num_pages; i++) {
        vmm_unmap_page(virt + (i * PAGE_SIZE), phys + (i * PAGE_SIZE));
    }
}