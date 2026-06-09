#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/slab.h>










void *alloc(size_t length);
void free(void *ptr);

#endif