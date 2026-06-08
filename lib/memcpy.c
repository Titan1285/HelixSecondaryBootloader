#include <string.h>










void memcpy(void *dst, void *src, size_t length) {
    uint8_t *d = NULL, *s = NULL;


    if (!dst || !src) {
        return;
    }

    for (size_t i = 0; i < length; i++) {
        d[i] = s[i];
    }
}