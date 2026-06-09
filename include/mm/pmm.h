#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.h>





extern uint8_t __ebss;



#define RAM_BASE    0x40000000
#define RAM_SIZE    0x20000000  // 512MB







typedef struct {
    uint64_t    ram_base;
    size_t      ram_size;

    size_t      total_pages;
    size_t      used_pages;
    size_t      free_pages;
    size_t      reserved_pages;

    uint64_t    last_used_idx;
    
    size_t      bitmap_size;
    uint8_t     *bitmap;
} pmm_t;







void pmm_init(void);
void *pmm_alloc(size_t num_pages);
void pmm_free(void *addr, size_t num_pages);

#endif