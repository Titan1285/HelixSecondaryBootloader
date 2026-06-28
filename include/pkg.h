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

typedef struct {
    pkg_header_t    *hdr;   // Main header
    pkg_table_t     *tbl;   // Table entries
    void            *data;  // Raw pkg file data
} pkg_t;















pkg_t *pkg_init(void *buf);
pkg_table_t *pkg_entry_for_each(pkg_t *pkg, uint32_t idx);
pkg_table_t *pkg_get_table_by_name(pkg_t *pkg, const char *s);

#endif