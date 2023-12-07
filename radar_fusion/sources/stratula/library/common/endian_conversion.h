#ifndef ENDIAN_CONVERSION_H
#define ENDIAN_CONVERSION_H 1

#include <stdint.h>
#include <string.h>


static inline void swap_endian_16(uint16_t *v)
{
    *v = (*v << 8) | (*v >> 8);
}


static inline void cpu_to_little_16(uint16_t *v)
{
#ifdef TARGET_PLATFORM_BIG_ENDIAN
    swap_endian_16(v);
#endif
}

static inline void cpu_to_big_16(uint16_t *v)
{
#ifndef TARGET_PLATFORM_BIG_ENDIAN
    swap_endian_16(v);
#endif
}

static inline void little_to_cpu_16(uint16_t *v)
{
    cpu_to_little_16(v);
}

static inline void big_to_cpu_16(uint16_t *v)
{
    cpu_to_big_16(v);
}


#ifdef TARGET_PLATFORM_BIG_ENDIAN

static inline void hostToLittle16(uint8_t buf[], uint16_t value)
{
    buf++ = (uint8_t)value;
    buf++ = value >> 8;
}

#else

static inline void hostToLittle16(uint8_t buf[], uint16_t value)
{
    memcpy(buf, &value, sizeof(value));
}

static inline uint16_t littleToHost16(const uint8_t buf[])
{
    uint16_t value;
    memcpy(&value, buf, sizeof(value));
    return value;
}

#endif


#define hostToBig(dst, src) hostToBigS(dst, &src, sizeof(src))
#define bigToHost(dst, src) bigToHostS(&dst, src, sizeof(dst))


/** Write a value to a big-endian buffer */
#define hostToBigV(dst, vsrc, type)             \
    {                                           \
        const type *src     = (type *)vsrc;     \
        const int_fast8_t s = 8 * (size - 1);   \
        for (int_fast8_t i = s; i >= 0; i -= 8) \
        {                                       \
            *(dst++) = (*src >> i);             \
        }                                       \
    }

static inline void hostToBigS(uint8_t dst[], const void *vsrc, size_t size)
{
    switch (size)
    {
        case 1:
            hostToBigV(dst, vsrc, uint8_t);
            break;
        case 2:
            hostToBigV(dst, vsrc, uint16_t);
            break;
        case 4:
            hostToBigV(dst, vsrc, uint32_t);
            break;
        case 8:
            hostToBigV(dst, vsrc, uint64_t);
            break;
        default:
            // error
            break;
    }
}

/** Read a value from a big-endian buffer */
#define bigToHostV(vdst, src, type)                 \
    {                                               \
        type *dst           = (type *)vdst;         \
        const int_fast8_t s = 8 * (size - 1);       \
        *dst                = *src << s;            \
        for (int_fast8_t i = s - 8; i >= 0; i -= 8) \
        {                                           \
            *dst |= *(++src) << i;                  \
        }                                           \
    }

static inline void bigToHostS(void *vdst, const uint8_t src[], size_t size)
{
    switch (size)
    {
        case 1:
            bigToHostV(vdst, src, uint8_t);
            break;
        case 2:
            bigToHostV(vdst, src, uint16_t);
            break;
        case 4:
            bigToHostV(vdst, src, uint32_t);
            break;
        case 8:
            bigToHostV(vdst, src, uint64_t);
            break;
        default:
            // error
            break;
    }
}


#endif /* ENDIAN_CONVERSION_H */
