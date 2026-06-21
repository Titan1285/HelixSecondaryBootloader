#include <mm/slab.h>
#include <mm/pmm.h>
#include <mm/alloc.h>














void *alloc(size_t length) {
    void *ptr = NULL;
    size_t num_pages = 0;




    if (length > 2048) {
        num_pages = SIZE_TO_PAGES(length);

        ptr = pmm_alloc(num_pages);

        if (!ptr) {
            return NULL;
        }

        return ptr;
    }

    // TODO: Fix slab allocator somehow allocating 2048 bytes in a 128 byte object...
    ptr = slab_alloc(length);

    if (!ptr) {
        return NULL;
    }

    return ptr;
}

void free(void *ptr) {
    if (!ptr) {
        return;
    }

    slab_free(ptr);
}