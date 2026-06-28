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

    pkg_t *pkg = pkg_init(logo);

    pkg_table_t *logo_pkg = pkg_get_table_by_name(pkg, "hboot_logo.bmp");

    // TODO: Make some pkg_decompress() function to handle this, and change how the data is fetched from the partition?
    uint8_t *logo_data = ((uint8_t *)logo + logo_pkg->data_offset);
    uint8_t *logo_raw = alloc(logo_pkg->raw_size);

    printf("Uncompressing: %s...\n", logo_pkg->name);
    
    unsigned int uz = (unsigned int)logo_pkg->raw_size;
    ret = tinf_zlib_uncompress(logo_raw, &uz, logo_data, logo_pkg->compressed_size);

    if (ret != TINF_OK) {
        printf("Failed to decompress: %s! (%d)\n", logo_pkg->name, ret);
        goto end;
    }
    
    // TODO: Use double buffer to display the image instead of rendering directly to the framebuffer (it's slow as fuck)
    bmp_display(logo_raw);

    uint32_t apps_bl_idx = gpt_partition_get_index("AppsBL");
    uint64_t apps_bl_offset = gpt_partition_get_offset(apps_bl_idx);
    size_t apps_bl_size = gpt_partition_get_size(apps_bl_idx);

    uint8_t *apps_bl = (uint8_t *)0x46000000;

    ret = sdhci_read_blocks(apps_bl_offset, apps_bl_size, apps_bl);

    if (ret < 0) {
        printf("Failed to read AppsBL from EMMC!\n");
        goto end;
    }

    arch_jump_apps_bl((uint64_t)apps_bl);

end:
    while(1);
    __builtin_unreachable();
}