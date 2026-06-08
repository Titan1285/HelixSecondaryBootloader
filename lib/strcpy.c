#include <string.h>












void strcpy(char *dst, const char *s) {
    if (!dst || !s) {
        return;
    }

    while(*s != '\0') {
        *dst = *s;

        dst++;
        s++;
    }
}