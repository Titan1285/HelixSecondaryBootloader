#include <stdio.h>
#include <mm/alloc.h>
#include <devices/gicv3.h>
#include <devices/sdhci.h>









int sbl_main(void) {

    printf("******************************\n");
    printf("* Helix Secondary Bootloader *\n");
    printf("******************************\n\n");

    pmm_init();
    slab_init();
    //gic_init();
    sdhci_init();


    while(1);
}