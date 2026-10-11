#pragma once
#include <stddef.h>

inline int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

inline int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0;
    }

    while (n > 1 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

inline size_t strlen(const char* str)
{
    size_t length = 0;

    while (str[length] != '\0')
        length++;

    return length;
}