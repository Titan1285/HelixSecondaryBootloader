#ifndef MBR_H
#define MBR_H

#include <stdint.h>


typedef struct {
    uint8_t     attributes;
    uint8_t     chs_start[3];
    uint8_t     partition_type;
    uint8_t     chs_end[3];
    uint32_t    lba_start;
    uint32_t    num_sectors;
} __attribute__((packed)) mbr_partition_t;

typedef struct {
    uint8_t         bootstrap[440];
    uint32_t        disk_signature;
    uint16_t        reserved;
    mbr_partition_t partitions[4];
    uint16_t        boot_signature;
} __attribute__((packed)) mbr_t;










void mbr_init(void);
mbr_partition_t *mbr_partition_get_by_index(uint32_t idx);

#endif