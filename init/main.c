#include <stdio.h>
#include <mm/alloc.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/sdhci.h>
#include <devices/timer.h>











int sbl_main(void) {

    printf("******************************\n");
    printf("* Helix Secondary Bootloader *\n");
    printf("******************************\n\n");

    pmm_init();
    slab_init();
    timer_init();
    sdhci_init();

    printf("Testing timer...\n");
    
    timer_set_phys(10000);
    timer_phys_enable();

    printf("???\n");

    while(1);
}