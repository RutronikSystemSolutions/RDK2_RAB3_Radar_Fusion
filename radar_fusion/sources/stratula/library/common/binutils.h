#ifndef BINUTILS_H
#define BINUTILS_H 1

#include <stdint.h>


inline static unsigned int swapBits(unsigned int n, unsigned int a, unsigned int b)
{
    // if both bits are different, then toggle both, else just return same value
    return (((n >> a) ^ (n >> b)) & 1) ? (n ^ (1 << a) ^ (1 << b)) : n;
}

inline static unsigned int maskBits(unsigned int bits, unsigned int value)
{
    return ((1u << bits) - 1) & value;
}

inline static unsigned int getBitCount(unsigned int value)
{
    unsigned int count = 0;
    while (value)
    {
        value &= (value - 1);
        count++;
    }
    return count;
}

inline static unsigned int getHighestBit(unsigned int value)
{
    unsigned int count = 0;
    while (value >>= 1)
    {
        count++;
    }
    return count;
}

#endif /* BINUTILS_H */
