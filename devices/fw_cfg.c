#include <stdio.h>
#include <arch/arch.h>
#include <devices/fw_cfg.h>









static inline void fw_cfg_write64(uint64_t offset, uint64_t v) {
    *(volatile uint64_t *)((uintptr_t)FW_CFG_BASE + offset) = __builtin_bswap64(v);
}

static void fw_cfg_dma_transfer(void *addr, uint32_t len, uint32_t ctrl) {
    volatile fw_cfg_dma_t dma_cfg;


    if (!addr || len == 0) {
        return;
    }
    
    dma_cfg.addr = __builtin_bswap64((uint64_t)addr);
    dma_cfg.size = __builtin_bswap32(len);
    dma_cfg.ctrl = __builtin_bswap32(ctrl);

    fw_cfg_write64(FW_CFG_DMA, (uint64_t)&dma_cfg);

    while(__builtin_bswap32(dma_cfg.ctrl) & ~FW_CFG_DMA_ERROR);
}

void fw_cfg_read(void *ptr, uint32_t len) {
    fw_cfg_dma_transfer(ptr, len, FW_CFG_DMA_READ);
}

void fw_cfg_read_entry(void *ptr, uint32_t entry, uint32_t len) {
    uint32_t ctrl = (entry << 16) | 0x08 | 0x02;
    
    fw_cfg_dma_transfer(ptr, len, ctrl);
}

void fw_cfg_write_entry(void *ptr, uint32_t entry, uint32_t len)  {
    uint32_t ctrl = (entry << 16) | 0x08 | 0x10;

    fw_cfg_dma_transfer(ptr, len, ctrl);
}

int fw_cfg_find_file(const char *s) {
    uint32_t count = 0, entry = 0;
    uint16_t sel = 0;


    fw_cfg_read_entry(&count, FW_CFG_FILE_DIR, sizeof(count));
    count = __builtin_bswap32(count);

    for (sel = 0, entry = 0; entry < count; entry++) {
        fw_cfg_file_t f;

        fw_cfg_read(&f, sizeof(fw_cfg_file_t));

        if (__builtin_strcmp(f.name, s) == 0) {
            sel = __builtin_bswap16(f.key_sel);
            break;
        }
    }

    return sel;
}