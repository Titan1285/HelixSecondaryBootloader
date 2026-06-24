#include <stdio.h>
#include <string.h>
#include <mm/alloc.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/sdhci.h>
#include <devices/timer.h>
#include <mbr.h>
#include <gpt.h>
#include <gfx/gfx.h>




















void sbl_main(void) {

    printf("******************************\n");
    printf("* Helix Secondary Bootloader *\n");
    printf("******************************\n\n");

    pmm_init();
    vmm_init();
    slab_init();
    timer_init();
    sdhci_init();
    mbr_init();
    gpt_init();

    
    uint32_t misc_idx = gpt_partition_get_index("Misc");
    uint64_t misc_offset = gpt_partition_get_offset(misc_idx);

    uint8_t *ptr = alloc(512);

    memset(ptr, 0xFF, 512);

    int ret = sdhci_write_block(misc_offset, ptr);

    if (ret < 0) {
        printf("Failed to write block!\n");
    } else {
        printf("Wrote block!\n");
    }

    while(1);
    __builtin_unreachable();
}