#ifndef KSTRING_H
#define KSTRING_H

#include <stdint.h>
#include <stddef.h>
















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

#endif