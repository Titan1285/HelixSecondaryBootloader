#include <string.h>










void memcpy(void *dst, void *src, size_t length) {
    uint8_t *d = NULL, *s = NULL;


    if (!dst || !src) {
        return;
    }

    // Ensure these are initialized
    d = (uint8_t *)dst;
    s = (uint8_t *)src;

    for (size_t i = 0; i < length; i++) {
        d[i] = s[i];
    }
}