#ifndef PKG_H
#define PKG_H

#include <stdint.h>
#include <stddef.h>









typedef struct {
    uint8_t     signature[4];
    uint64_t    entry_count;
    uint64_t    table_offset;
} __attribute__((packed)) pkg_header_t;

typedef struct {
    size_t      raw_size;
    size_t      compressed_size;
    uint64_t    data_offset;
    char        name[128];
} __attribute__((packed)) pkg_table_t;

#endif