#include <stdio.h>
#include <mm/alloc.h>
#include <devices/sdhci.h>
#include <mbr.h>






static mbr_t *mbr;






void mbr_init(void) {
    int ret = 0;
    

    mbr = alloc(sizeof(mbr_t));

    if (!mbr) {
        printf("Failed to allocate MBR header!\n");
        return;
    }

    ret = sdhci_read_block(0, (uint8_t *)mbr);

    if (ret < 0) {
        printf("Failed to read MBR!\n");
        return;
    }

    if (mbr->boot_signature != 0xAA55) {
        printf("Invalid MBR signature: %lx\n", mbr->boot_signature);
        return;
    }
}

mbr_partition_t *mbr_partition_get_by_index(uint32_t idx) {
    mbr_partition_t *p = NULL;



    if (idx > 4) {
        printf("Partition index %lu out of bounds!\n", idx);
        return NULL;
    }

    p = &mbr->partitions[idx];

    if (!p) {
        return NULL;
    }

    return p;
}