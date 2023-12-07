#include "memcpy32.h"


void memcpy32(void *dest, const void *src, uint32_t length)
{
    const uintptr_t destU     = (uintptr_t)dest;
    const uint32_t firstBytes = (length < sizeof(uint32_t)) ? length : (sizeof(uint32_t) - (destU & 0x03)) & 0x03;
    uint32_t *dest32          = (uint32_t *)(destU + firstBytes);
    length -= firstBytes;

    // first bytes until dest is 32-bit aligned
    uint8_t *dest8      = dest;
    const uint8_t *src8 = src;
    while (dest8 < (uint8_t *)dest32)
    {
        *(dest8++) = *(src8++);
    }

    // write 32-bit words
    uint8_t *dest8End        = dest8 + length;
    const uint32_t wordCount = length / sizeof(uint32_t);
    uint32_t *dest32end      = dest32 + wordCount;
    const uint8_t *src8End   = src8 + wordCount * sizeof(uint32_t);

    const uintptr_t srcOffset = (uintptr_t)src8 & 0x03;
    uint32_t *src32           = (uint32_t *)((uintptr_t)src8 - srcOffset);
    uint32_t srcWord1, srcWord2;

    switch (srcOffset)
    {
        case 0:  // src is 32-bit aligned
            while (dest32 < dest32end)
            {
                *(dest32++) = *(src32++);
            }
            break;
        case 1:  // src points to byte 1 of a 32-bit word
            srcWord1 = *(src32++);
            while (dest32 < dest32end)
            {
                srcWord2    = *(src32++);
                *(dest32++) = (srcWord1 >> 8) | (srcWord2 << 24);
                srcWord1    = srcWord2;
            }
            break;
        case 2:  // src points to byte 2 of a 32-bit word
            srcWord1 = *(src32++);
            while (dest32 < dest32end)
            {
                srcWord2    = *(src32++);
                *(dest32++) = (srcWord1 >> 16) | (srcWord2 << 16);
                srcWord1    = srcWord2;
            }
            break;
        case 3:  // src points to byte 3 of a 32-bit word
            srcWord1 = *(src32++);
            while (dest32 < dest32end)
            {
                srcWord2    = *(src32++);
                *(dest32++) = (srcWord1 >> 24) | (srcWord2 << 8);
                srcWord1    = srcWord2;
            }
            break;
    }

    // write remaining bytes
    dest8 = (uint8_t *)dest32;
    while (dest8 < dest8End)
    {
        *(dest8++) = *(src8End++);
    }
}
