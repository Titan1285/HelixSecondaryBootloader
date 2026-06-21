#include <stdio.h>
#include <string.h>
#include <mm/alloc.h>
#include <pkg.h>
#include <devices/sdhci.h>
#include <mbr.h>
#include <gpt.h>


















pkg_t *pkg_open(void) {
    return NULL;
}

int pkg_read(pkg_t *pkg, uint64_t offset, size_t length, void *buf) {
    return 0;
}

pkg_table_t *pkg_table_for_each(pkg_t *pkg, uint32_t idx) {
    return NULL;
}