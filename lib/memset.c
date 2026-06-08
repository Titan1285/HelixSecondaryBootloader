#include <string.h>







void memset(void *dst, char ch, size_t length) {
    uint8_t *ptr = NULL;
    


    if (!dst) {
        return;
    }

    ptr = (uint8_t *)dst;

    for (size_t i = 0; i < length; i++) {
        ptr[i] = ch;
    }
}