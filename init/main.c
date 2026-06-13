#include <stdio.h>
#include <mm/alloc.h>
#include <arch/arch.h>
#include <devices/gicv3.h>
#include <devices/sdhci.h>
#include <devices/timer.h>
#include <mbr.h>
#include <gpt.h>













void sbl_main(void) {
    int ret = 0;

    printf("******************************\n");
    printf("* Helix Secondary Bootloader *\n");
    printf("******************************\n\n");

    pmm_init();
    slab_init();
    timer_init();
    sdhci_init();


    mbr_t *mbr = alloc(sizeof(mbr_t));

    if (!mbr) {
        printf("Failed to allocate MBR header!\n");
        goto end;
    }

    ret = sdhci_read_block(0, (uint8_t *)mbr);

    if (ret < 0) {
        printf("Failed to read MBR from EMMC!\n");
        goto end;
    }

    if (mbr->boot_signature != 0xAA55) {
        printf("Invalid MBR boot signature: %x\n", mbr->boot_signature);
        goto end;
    }

    mbr_partition_t *p = &mbr->partitions[0];

    if (p->partition_type != 0xEE) {
        printf("Partition type not GPT header!\n");
        goto end;
    }

    gpt_t *gpt = alloc(sizeof(gpt_t));

    if (!gpt) {
        printf("Failed to allocate GPT header!\n");
        goto end;
    }

    ret = sdhci_read_blocks(p->lba_start, sizeof(gpt_t), (uint8_t *)gpt);

    if (ret < 0) {
        printf("Failed to read GPT from EMMC!\n");
        goto end;
    }

    size_t gpt_partition_arr_size = gpt->num_partitions * gpt->partition_entry_size;

    gpt_partition_t *gpt_parts = alloc(gpt_partition_arr_size);

    if (!gpt_parts) {
        printf("Failed to allocate GPT partition array!\n");
        goto end;
    }

    ret = sdhci_read_blocks(gpt->partition_entries_lba, gpt_partition_arr_size, (uint8_t *)gpt_parts);

    if (ret < 0) {
        printf("Failed to read GPT partition array from EMMC!\n");
        goto end;
    }



    gpt_partition_t *dtb_part = (gpt_partition_t *)((uint8_t *)gpt_parts + (3 * gpt->partition_entry_size));

    size_t dtb_size = (dtb_part->lba_end - dtb_part->lba_start + 1) * 512;

    // TODO: Don't do this, or hardcode it...
    uint8_t *dtb = (uint8_t *)0x48000000;

    ret = sdhci_read_blocks(dtb_part->lba_start, dtb_size, dtb);

    if (ret < 0) {
        printf("Failed to read DTB from EMMC!\n");
        goto end;
    }

    gpt_partition_t *ub = (gpt_partition_t *)((uint8_t *)gpt_parts + (1 * gpt->partition_entry_size));

    size_t ub_size = (ub->lba_end - ub->lba_start + 1) * 512;

    uint8_t *ub_ptr = alloc(ub_size);

    if (!ub_ptr) {
        printf("Failed to allocate UB partition!\n");
        goto end;
    }

    ret = sdhci_read_blocks(ub->lba_start, ub_size, ub_ptr);

    if (ret < 0) {
        printf("Failed to read UB from EMMC!\n");
        goto end;
    }

    arch_jump_ub((uint64_t)ub_ptr);

end:
    while(1);
    __builtin_unreachable();
}