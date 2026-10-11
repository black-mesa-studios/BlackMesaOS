#include <stdint.h>
#include <stddef.h>

void copy(uint8_t *dst, const uint8_t *source, size_t length) {
    for(size_t i = 0; i < length; i++) {
        dst[i] = source[i];
    }
}