#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mm/alloc.h>
#include <pkg.h>
#include <devices/sdhci.h>













pkg_t *pkg_init(void *buf) {
    pkg_t *pkg = NULL;
    


    if (!buf) {
        return NULL;
    }

    // NOTE: Maybe not the best, but it works...
    pkg = alloc(sizeof(pkg_t));

    if (!pkg) {
        return NULL;
    }

    pkg->hdr = (pkg_header_t *)buf;
    pkg->tbl = (pkg_table_t *)(buf + pkg->hdr->table_offset);

    return pkg;
}

pkg_table_t *pkg_entry_for_each(pkg_t *pkg, uint32_t idx) {
    pkg_table_t *p = NULL;



    if (!pkg) {
        return NULL;
    }

    if (idx > pkg->hdr->entry_count) {
        printf("Entry index %lu exceeds %lu!\n", idx, pkg->hdr->entry_count);
        return NULL;
    }

    p = &pkg->tbl[idx];

    return p;
}

pkg_table_t *pkg_get_table_by_name(pkg_t *pkg, const char *s) {
    pkg_table_t *p = NULL;


    if (!pkg || !s) {
        return NULL;
    }

    for (uint32_t i = 0; i < pkg->hdr->entry_count; i++) {
        pkg_table_t *pt = pkg_entry_for_each(pkg, i);

        if (strcmp(pt->name, s) == 0) {
            p = pt;
            break;
        }
    }

    return p;
}