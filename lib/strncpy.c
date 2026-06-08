#include <string.h>











void strncpy(char *dst, const char *s, size_t length) {
    if (!dst || !s) {
        return;
    }
    

    for (size_t i = 0; i < length; i++) {
        dst[i] = s[i];
    }
}