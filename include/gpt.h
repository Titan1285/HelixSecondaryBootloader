#ifndef GPT_H
#define GPT_H

#include <stdint.h>








typedef struct {
    uint32_t  time_low;
    uint16_t  time_mid;
    uint16_t  time_high_and_ver;
    uint8_t   clock_seq_high_and_res;
    uint8_t   clock_seq_low;
    uint8_t   node[6];
} __attribute__((packed)) guid_t;
typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc32;
    uint32_t reserved;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    guid_t   disk_guid;
    uint64_t partition_entries_lba;
    uint32_t num_partitions;
    uint32_t partition_entry_size;
    uint32_t partition_crc32;
    uint8_t pad[420];
} __attribute__((packed)) gpt_t;
typedef struct {
    guid_t      partition_type;
    guid_t      partition_guid;
    uint64_t    lba_start;
    uint64_t    lba_end;
    uint64_t    attributes;
    uint8_t     partition_name[32];
} __attribute__((packed)) gpt_partition_t;

#endif