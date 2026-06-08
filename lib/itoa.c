#include <string.h>
#include <stdlib.h>
#include <stdbool.h>













void itoa(int val, void *buf, int base) {
    int i = 0;
    bool is_negitive = 0;
    char *ptr = NULL;

    

    if (!buf) {
        return;
    }

    // TODO: Confirm base 36 for signed/unsigned ints???
    if (base < 2 || base > 36) {
        // Invalid base (can only be 2-36)
        return;
    }

    ptr = (char *)buf;

    if (val == 0) {
        ptr[i++] = '0';
        ptr[i] = '\0';

        return;
    }

    if (val < 0 && base == 10) {
        is_negitive = true;
        val = -val;
    }

    while(val != 0) {
        int rem = val % base;

        ptr[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        val = val / base;
    }

    if (is_negitive) {
        ptr[i++] = '-';
    }

    ptr[i] = '\0';

    strrev(ptr, strlen(ptr));

    return;
}