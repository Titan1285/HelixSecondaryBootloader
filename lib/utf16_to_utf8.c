#include <string.h>



void utf16_to_utf8(uint8_t *dest, const uint16_t *src, size_t max_utf16_chars) {
    uint32_t code_point;
    size_t i = 0;

    while (i < max_utf16_chars && src[i] != 0) {
        code_point = le16_to_cpu(src[i]); 
        i++;

        if (code_point <= 0x7F) {
            *dest++ = (uint8_t)code_point;
        
        } else if (code_point <= 0x7FF) {
            *dest++ = (uint8_t)(0xC0 | (code_point >> 6));
            *dest++ = (uint8_t)(0x80 | (code_point & 0x3F));
        
        } else if (code_point <= 0xFFFF) {
            *dest++ = (uint8_t)(0xE0 | (code_point >> 12));
            *dest++ = (uint8_t)(0x80 | ((code_point >> 6) & 0x3F));
            *dest++ = (uint8_t)(0x80 | (code_point & 0x3F));
        }
    }

    *dest = '\0';
}