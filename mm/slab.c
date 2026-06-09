#include <stdio.h>
#include <string.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/slab.h>
#include <stdbool.h>







static uint64_t slab_phys_base = SLAB_PHYS_BASE;
static uint64_t slab_phys_end = SLAB_PHYS_END;

static slab_cache_t slab_cache[SLAB_CACHE_SIZE];






static inline bool slab_test_bit(uint8_t *bitmap, uint64_t idx) {
    return bitmap[idx / 8] & (1ULL << (idx % 8));
}

static inline void slab_set_bit(uint8_t *bitmap,uint64_t idx) {
    bitmap[idx / 8] |= (1ULL << (idx % 8));
}

static inline void slab_clear_bit(uint8_t *bitmap, uint64_t idx) {
    bitmap[idx / 8] &= ~(1ULL << (idx % 8));
}

static int slab_get_order(size_t slab_size) {
    int slab_max_order = SLAB_ORDER_MAX;
    uint64_t slab_min_size  = SLAB_SIZE_BASE;

    // Reject invalid sizes
    if (slab_size == 0 || slab_size > SLAB_SIZE_MAX) {
        return -1;
    }

    for (int i = 0; i <= slab_max_order; i++) {
        if (slab_size <= slab_min_size) {
            return i;  // order index
        }

        slab_min_size <<= 1;
    }

    // Size too large for slab allocator
    return -1;
}

static void slab_link(slab_cache_t *cache, slab_t *slab) {
    slab_t *prev = NULL;

    
    if (!cache || !slab) {
        return;
    }

    if (!cache->slab) {
        cache->slab = slab;
        return;
    }

    prev = cache->slab;

    while(prev->next != NULL) {
        prev = prev->next;
    }

    prev->next = slab;
}

static void *slab_get_object(slab_cache_t *cache, slab_t *slab) {
    void *ptr = NULL;
    

    if (!cache || !slab) {
        return NULL;
    }

    for (uint64_t i = 0; i < slab->total_objs; i++) {
        bool is_obj_free = !slab_test_bit(slab->bitmap, i);

        if (is_obj_free) {
            // Got free object, allocate...
            ptr = (uint8_t *)slab->slab_base + (i * cache->obj_size);

            slab->free_objs--;
            slab->used_objs++;

            // Ensure this object is marked as used
            slab_set_bit(slab->bitmap, i);

            printf("SLAB: Allocated slab object @ %llx, length = %llu, max_object_size = %llu\n", (uint64_t)ptr, slab->obj_size, cache->obj_size);

            return ptr;
        }
    }

    return NULL;
}

void slab_init(void) {
    uint64_t slab_size_base = SLAB_SIZE_BASE;
    uint64_t slab_max_order = SLAB_ORDER_MAX;


    for (uint64_t i = 0; i <= slab_max_order; i++) {
        uint64_t slab_size = slab_size_base << i;
        slab_cache_t *cache = (slab_cache_t *)&slab_cache[i];
        
        slab_t *slab = slab_create_new(slab_size);

        if (!slab) {
            printf("SLAB: Failed to create slab size %llu!\n", slab_size);
            return;
        }

        if (!cache->slab) {
            cache->slab = slab;
            cache->obj_size = slab_size;
        }
    }
}

slab_t *slab_create_new(size_t obj_size) {
    slab_t *slab = NULL;
    size_t page_size = PAGE_SIZE;
    uint64_t est_objs = (page_size - sizeof(slab_t)) / obj_size;
    size_t slab_bitmap_size = (est_objs + 7) / 8;
    uint64_t slab_objs_offset = ALIGN_UP(sizeof(slab_t) + slab_bitmap_size, 16);
    size_t free_objs = (page_size - slab_objs_offset) / obj_size;


    

    slab = pmm_alloc(1);

    if (!slab) {
        return NULL;
    }

    memset(slab, 0, PAGE_SIZE);

    // Ensure we're able to map this page within the slab range (TODO: move the slab range away from the end of the kernel)
    if (slab_phys_base >= slab_phys_end) {
        return NULL;
    }

    slab->obj_size = obj_size;
    slab->used_objs = 0;
    slab->free_objs = free_objs;
    slab->total_objs = slab->free_objs;
    slab->next = NULL;

    slab->bitmap = (uint8_t *)(slab + 1);
    slab->slab_base = (void *)((uint8_t *)slab_phys_base + slab_objs_offset);
    
    // Ensure bitmap is cleared
    memset(slab->bitmap, 0, slab_bitmap_size);

    // TODO: Improve from doing this...
    slab_phys_base += PAGE_SIZE;

    return slab;
}

void *slab_alloc(size_t length) {
    int slab_order = 0;
    slab_cache_t *cache = NULL;
    slab_t *slab = NULL;



    if (length < SLAB_SIZE_BASE) {
        length = SLAB_SIZE_BASE;
    }

    if (length > SLAB_SIZE_MAX) {
        return NULL;
    }

    slab_order = slab_get_order(length);

    // Get allocated slab objects from <slab_order>
    cache = (slab_cache_t *)&slab_cache[slab_order];

    if (!cache) {
        printf("SLAB: No slab cache created!\n");
        return NULL;
    }

    slab = cache->slab;

    if (slab->free_objs == 0) {
        // Check for existing slab in cache to allocate, if any...
        slab_t *prev = slab;

        while(prev != NULL) {
            if (prev->free_objs > 0) {
                // Found slab to fit allocation request, allocate...
                return slab_get_object(cache, prev);
                break;
            }

            prev = prev->next;
        }

        if (!prev) {
            // Create and link new slab for this allocation request
            slab_t *new_slab = slab_create_new(length);

            if (!slab) {
                printf("SLAB: Failed to create new slab object!\n");
                return NULL;
            }

            // Ensure this slab is linked to the existing cache
            slab_link(cache, new_slab);

            // Allocate this slab...
            return slab_get_object(cache, new_slab);
        }
    } else {
        // Allocate from existing slab
        return slab_get_object(cache, slab);
    }

    printf("SLAB: No available slabs for allocation!\n");

    return NULL;
}

void slab_free(void *ptr) {
    slab_cache_t *cache = NULL;
    slab_t *slab = NULL;
    uint64_t idx = 0;
    int slab_order = 0;



    if (!ptr) {
        return;
    }

    slab = (slab_t *)ALIGN_DOWN((uint64_t)ptr, PAGE_SIZE);

    slab_order = slab_get_order(slab->obj_size);
    cache = (slab_cache_t *)&slab_cache[slab_order];

    printf("SLAB: Freeing slab object @ %llx, length = %llu, max_object_size = %llu\n", (uint64_t)ptr, slab->obj_size, cache->obj_size);

    idx = ((uint64_t)ptr - (uint64_t)slab->slab_base) / slab->obj_size;

    if (slab_test_bit(slab->bitmap, idx)) {
        // Free object
        slab->free_objs++;
        slab->used_objs--;

        slab_clear_bit(slab->bitmap, idx);
    }
}