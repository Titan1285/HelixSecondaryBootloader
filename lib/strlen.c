#include <string.h>










size_t strlen(const char *s) {
    size_t length = 0;


    if (!s) {
        return -1;
    }

    while(*s != '\0') {
        length++;
        s++;
    }
    
    return length;
}