#ifndef SLAB_H
#define SLAB_H

#include <stdint.h>
#include <stddef.h>



#define SLAB_CACHE_SIZE     8

#define SLAB_ORDER_MIN      0
#define SLAB_ORDER_MAX      5

#define SLAB_SIZE_BASE      64ULL
#define SLAB_SIZE_MAX       2048ULL

#define SLAB_SIZE           (65536 * 1024)
#define SLAB_PHYS_BASE      0x40100000
#define SLAB_PHYS_END       (SLAB_PHYS_BASE + SLAB_SIZE)






typedef struct _slab {
    struct _slab    *next;
    size_t          used_objs;
    size_t          free_objs;
    size_t          total_objs;
    size_t          obj_size;
    uint8_t         *bitmap;
    uint8_t         *slab_base;
} slab_t;

typedef struct {
    size_t  obj_size;
    slab_t  *slab;
} slab_cache_t;






void slab_init(void);
slab_t *slab_create_new(size_t obj_size);
void *slab_alloc(size_t length);
void slab_free(void *ptr);

#endif 