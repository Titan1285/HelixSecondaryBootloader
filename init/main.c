#include <stdio.h>
#include <mm/alloc.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/sdhci.h>
#include <devices/timer.h>
#include <mbr.h>
#include <gpt.h>
#include <gfx/gfx.h>
#include <pkg.h>



















void sbl_main(void) {

    printf("******************************\n");
    printf("* Helix Secondary Bootloader *\n");
    printf("******************************\n\n");

    pmm_init();
    slab_init();
    timer_init();
    sdhci_init();
    mbr_init();
    gpt_init();
    

    while(1);
    __builtin_unreachable();
}