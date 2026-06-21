#ifndef FW_CFG_H
#define FW_CFG_H

#include <stdint.h>
#include <stddef.h>
#include <arch/arch.h>




#define FW_CFG_BASE     0x01041000

#define FW_CFG_SEL      0x08
#define FW_CFG_DATA     0x00
#define FW_CFG_DMA      0x10

#define FW_CFG_DMA_ERROR    0x01
#define FW_CFG_DMA_READ     0x02
#define FW_CFG_DMA_SKIP     0x04
#define FW_CFG_DMA_SEL      0x08
#define FW_CFG_DMA_WRITE    0x10

#define FW_CFG_FILE_DIR     0x19








typedef struct {
    uint32_t    size;
    uint16_t    key_sel;
    uint16_t    reserved;
    char        name[56];
} __attribute__((packed)) fw_cfg_file_t;

typedef struct {
    uint32_t        count;
    fw_cfg_file_t   files[];
} fw_cfg_files_t;

typedef struct {
    uint64_t    addr;
    uint32_t    fourcc;
    uint32_t    flags;
    uint32_t    width;
    uint32_t    height;
    uint32_t    stride;
} __attribute__((packed)) fw_ramfb_cfg_t;

typedef struct {
    uint32_t    ctrl;
    uint32_t    size;
    uint64_t    addr;
} __attribute__((packed)) fw_cfg_dma_t;












void fw_cfg_read(void *ptr, uint32_t len);
void fw_cfg_read_entry(void *ptr, uint32_t entry, uint32_t len);
void fw_cfg_write_entry(void *ptr, uint32_t entry, uint32_t len);
int fw_cfg_find_file(const char *s);

#endif