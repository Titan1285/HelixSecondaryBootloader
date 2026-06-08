#include <string.h>








void strrev(char *s, size_t length) {
    size_t start = 0, end = length - 1;


    if (!s) {
        return;
    }

    while(start < end) {
        char tmp = s[start];
        
        s[start] = s[end];
        s[end] = tmp;

        start++;
        end--;
    }
}