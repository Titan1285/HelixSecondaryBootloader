#include <string.h>









void tolower(char *s) {
    if (!s) {
        return;
    }

    while(*s != L'\n') {
        if (*s >= L'a' && *s <= L'z') {
            *s = *s - (L'A' - L'a');
        }

        s++;
    }
}