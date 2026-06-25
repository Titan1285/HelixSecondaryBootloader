#ifndef KSTDLIB_H
#define KSTDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>




typedef __uint128_t uint128_t;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define __assert_fail(...)





void itoa(int val, void *buf, int base);
void utoa(unsigned int val, void *buf, int base);
void utoa64(unsigned long long val, void *buf, int base);

#endif