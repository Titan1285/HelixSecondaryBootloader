#include <stdio.h>
#include <mm/pmm.h>
#include <stdbool.h>
#include <string.h>







static pmm_t pmm;







static inline bool pmm_test_bit(uint64_t idx) {
    return pmm.bitmap[idx / 8] & (1ULL << (idx % 8));
}

static inline void pmm_set_bit(uint64_t idx) {
    pmm.bitmap[idx / 8] |= (1ULL << (idx % 8));
}

static inline void pmm_clear_bit(uint64_t idx) {
    pmm.bitmap[idx / 8] &= ~(1ULL << (idx % 8));
}




void pmm_init(void) {
    size_t bitmap_pages = 0;


    pmm.ram_base = RAM_BASE;
    pmm.ram_size = RAM_SIZE;

    pmm.total_pages = pmm.ram_size / PAGE_SIZE;
    pmm.free_pages = 0;
    pmm.used_pages = 0;
    pmm.last_used_idx = 0;

    pmm.bitmap_size = PAGE_ALIGN_UP((pmm.total_pages + 7) / 8);
    pmm.reserved_pages = pmm.bitmap_size / PAGE_SIZE;

    // Bitmap is placed at beginning of RAM
    pmm.bitmap = (uint8_t *)&__ebss;
    memset((uint8_t *)pmm.bitmap, 0xFF, pmm.bitmap_size);

    bitmap_pages = pmm.bitmap_size / PAGE_SIZE;

    for (size_t i = 0; i < bitmap_pages; i++) {
        pmm_set_bit(i);
    }
    
    size_t first_free = pmm.reserved_pages;
    size_t last_free = pmm.total_pages;

    for (size_t i = first_free; i < last_free; i++) {
        pmm_clear_bit(i);
    }

    pmm.used_pages = first_free;
    pmm.free_pages = pmm.total_pages - first_free;
    pmm.last_used_idx = first_free;

    printf("PMM information:\n");
    printf("RAM base:       %llx\n", pmm.ram_base);
    printf("RAM end:        %llx\n", pmm.ram_base + pmm.ram_size);
    printf("RAM size:       %lluMB\n", pmm.ram_size / (1024 * 1024));

    printf("Total pages:    %llu\n", pmm.total_pages);
    printf("Used pages:     %llu\n", pmm.used_pages);
    printf("Free pages:     %llu\n", pmm.free_pages);
    printf("Reserved pages: %llu\n", pmm.reserved_pages);

    printf("Bitmap size:    %lluKB\n", pmm.bitmap_size / 1024);
    printf("Bitmap base:    %llx\n", (uint64_t)pmm.bitmap);
}

static void *pmm_inner_alloc(size_t num_pages, size_t limit) {
    size_t run_start = 0;
    size_t run_length = 0;



    
    while (pmm.last_used_idx < limit) {
        if (!pmm_test_bit(pmm.last_used_idx)) {
            if (run_length == 0) {
                run_start = pmm.last_used_idx;
            }

            run_length++;

            if (run_length >= num_pages) {
                for (size_t i = run_start; i < run_start + num_pages; i++) {
                    pmm_set_bit(i);
                }

                return (void *)(pmm.ram_base + (run_start * PAGE_SIZE));
            }
        } else {
            run_length = 0;
        }

        pmm.last_used_idx++;
    }

    return NULL;
}
void *pmm_alloc(size_t num_pages) {
    void *ptr = NULL;


    if (pmm.free_pages == 0) {
        printf("PMM: no available pages!\n");
        return NULL;
    }

    ptr = pmm_inner_alloc(num_pages, pmm.total_pages);

    if (!ptr) {
        pmm.last_used_idx = 0;
        ptr = pmm_inner_alloc(num_pages, pmm.total_pages);

        if (!ptr) {
            printf("Failed to allocate page frame!\n");
            return NULL;
        }
    }

    pmm.used_pages += num_pages;
    pmm.free_pages -= num_pages;

    return ptr;
}

void pmm_free(void *addr, size_t num_pages) {
    uint64_t phys = (uint64_t)addr;
    size_t page = (phys - pmm.ram_base) / PAGE_SIZE;

    // Ensure we're not trying to free more than what's available
    if (pmm.used_pages <= pmm.reserved_pages) {
        printf("PMM: Non-valid memory attempted to be freed!\n");
        return;
    }

    for (uint64_t i = page; i < page + num_pages; i++) {
        pmm_clear_bit(i);
    }

    pmm.used_pages -= num_pages;
    pmm.free_pages += num_pages;
}