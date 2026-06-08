#include <string.h>








void toupper(char *s) {
    if (!s) {
        return;
    }

    while(*s != L'\n') {
        if (*s >= L'a' && *s <= L'z') {
            *s = *s - (L'a' - L'A');
        }

        s++;
    }
}