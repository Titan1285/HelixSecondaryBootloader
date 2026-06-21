#ifndef KSTRING_H
#define KSTRING_H

#include <stdint.h>
#include <stddef.h>
#include <uchar.h>




#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define le16_to_cpu(x) __builtin_bswap16(x)
#else
    #define le16_to_cpu(x) (x)
#endif














void memset(void *dst, char ch, size_t length);
void memcpy(void *dst, void *src, size_t length);
void memmove(void *dst, void *src, size_t length);
int memcmp(void *s1, void *s2, size_t length);
void strrev(char *s, size_t length);
size_t strlen(const char *s);
void strcpy(char *dst, const char *s);
void strncpy(char *dst, const char *s, size_t length);
void toupper(char *s);
void tolower(char *s);
int strcmp(const char *s1, const char *s2);
int strcmp16(char16_t *s1, char16_t *s2);
void utf16_to_utf8(uint8_t *dest, const uint16_t *src, size_t max_utf16_chars);

#endif