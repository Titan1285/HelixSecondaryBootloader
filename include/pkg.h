#ifndef PKG_H
#define PKG_H

#include <stdint.h>
#include <stddef.h>





#define PKG_NAME_LEN   128







typedef struct {
    uint8_t     magic[4];
    uint64_t    count;
    uint64_t    table_offset;
} __attribute__((packed)) pkg_header_t;

typedef struct {
    size_t      size;
    uint64_t    data_offset;
    char        name[PKG_NAME_LEN];
} __attribute__((packed)) pkg_table_t;

typedef struct {
    pkg_header_t    hdr;
    pkg_header_t    *tbl;
} pkg_t;







pkg_t *pkg_open(void);
int pkg_read(pkg_t *pkg, uint64_t offset, size_t length, void *buf);
pkg_table_t *pkg_table_for_each(pkg_t *pkg, uint32_t idx);

#endif