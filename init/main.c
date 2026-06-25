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
#include <gfx/bmp.h>
#include <tinf.h>
#include <pkg.h>

























void sbl_main(void) {
    int ret = 0;



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
    gfx_init();


    
    uint32_t logo_idx = gpt_partition_get_index("Logo");
    uint64_t logo_offset = gpt_partition_get_offset(logo_idx);
    unsigned int logo_size = gpt_partition_get_size(logo_idx);


    uint8_t *logo = alloc(logo_size);

    if (!logo) {
        printf("Failed to allocate boot logo buffer!\n");
        goto end;
    }

    ret = sdhci_read_blocks(logo_offset, logo_size, logo);

    if (ret < 0) {
        printf("Failed to read Logo partition!\n");
        goto end;
    }

    pkg_header_t *hdr = (pkg_header_t *)logo;

    printf("PKG magic:      %c%c%c\n", hdr->signature[0], hdr->signature[1], hdr->signature[2]);
    printf("Entry count:    %llu\n", hdr->entry_count);
    printf("Table offset:   %llx\n", hdr->table_offset);

    pkg_table_t *tbl = (pkg_table_t *)((uint8_t *)logo + hdr->table_offset);
    pkg_table_t *p = &tbl[0];

    uint8_t *logo_data = ((uint8_t *)logo + p->data_offset);
    uint8_t *logo_raw = alloc(p->raw_size);

    printf("Uncompressing: %s...\n", p->name);
    unsigned int uz = (unsigned int)p->raw_size;
    ret = tinf_zlib_uncompress(logo_raw, &uz, logo_data, p->compressed_size);

    if (ret != TINF_OK) {
        printf("Warning: Data extraction error! (%d)\n", ret);
    } else {
        printf("Extracted %s @ %llx\n", p->name, (uint64_t)logo_raw);
    }

    bmp_display(logo_raw);

end:
    while(1);
    __builtin_unreachable();
}