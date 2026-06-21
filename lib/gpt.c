#include <stdio.h>
#include <string.h>
#include <mm/alloc.h>
#include <devices/sdhci.h>
#include <mbr.h>
#include <gpt.h>
#include <uchar.h>






static gpt_t *gpt = NULL;
static gpt_partition_t *gpt_parts = NULL;


















void gpt_init(void) {
    int ret = 0;
    mbr_partition_t *gpt_entry = NULL;
    size_t gpt_part_arr_size = 0;




    gpt = alloc(sizeof(gpt_t));

    if (!gpt) {
        printf("Failed to allocate GPT header!\n");
        return;
    }

    gpt_entry = mbr_partition_get_by_index(0);

    if (gpt_entry->partition_type != 0xEE) {
        printf("Invalid GPT partition entry type: %lx\n", gpt_entry->partition_type);
        return;
    }

    ret = sdhci_read_block(gpt_entry->lba_start, (uint8_t *)gpt);

    if (ret < 0) {
        printf("Failed to read GPT header from EMMC!\n");
        return;
    }

    gpt_part_arr_size = gpt->num_partitions * gpt->partition_entry_size;

    gpt_parts = alloc(gpt_part_arr_size);

    if (!gpt_parts) {
        printf("Failed to allocate GPT partition entry array!\n");
        return;
    }

    ret = sdhci_read_blocks(gpt->partition_entries_lba, gpt_part_arr_size, (uint8_t *)gpt_parts);

    if (ret < 0) {
        printf("Failed to read GPT partition entry array from EMMC!\n");
        return;
    }
}

uint32_t gpt_partition_get_index(const char *s) {


    if (!s) {
        return NULL;
    }

    for (uint32_t i = 0; i < gpt->num_partitions; i++) {
        gpt_partition_t *p = (gpt_partition_t *)((uint8_t *)gpt_parts + (i * gpt->partition_entry_size));
        char name[36];



        if (p->lba_start == 0x0) {
            // Skip empty entries
            continue;
        }

        utf16_to_utf8((uint8_t *)name, (const char16_t *)p->partition_name, 36);

        if (strcmp(name, s) == 0) {
            return i;
        }
    }

    // Index not found
    return -1;
}

uint64_t gpt_partition_get_offset(uint32_t idx) {
    gpt_partition_t *p = NULL;



    if (idx > gpt->num_partitions) {
        printf("Partition index %lu out of bounds!\n", idx);
        return -1;
    }

    p = (gpt_partition_t *)((uint8_t *)gpt_parts + (idx * gpt->partition_entry_size));

    if (!p) {
        printf("Failed to get partition index %lu!\n", idx);
        return -1;
    }

    return p->lba_start;
}

size_t gpt_partition_get_size(uint32_t idx) {
    gpt_partition_t *p = NULL;
    size_t partition_size = 0;



    if (idx > gpt->num_partitions) {
        printf("Partition index %lu out of bounds!\n", idx);
        return -1;
    }

    p = (gpt_partition_t *)((uint8_t *)gpt_parts + (idx * gpt->partition_entry_size));

    if (!p) {
        printf("Failed to get partition index %lu!\n", idx);
        return -1;
    }

    partition_size = (p->lba_end - p->lba_start + 1) * 512;

    return partition_size;
}