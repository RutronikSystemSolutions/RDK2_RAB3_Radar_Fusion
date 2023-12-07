#ifndef SERIALIZATION_H
#define SERIALIZATION_H 1

#include "binutils.h"
#include <stdint.h>
#include <string.h>


static inline uint8_t serialToHost8(const uint8_t buf[])
{
    return *buf;
}

static inline void hostToSerial8(uint8_t buf[], uint8_t value)
{
    *buf = value;
}

static inline uint16_t serialToHost16(const uint8_t buf[])
{
    return (buf[1] << 8) | buf[0];
}

static inline void hostToSerial16(uint8_t buf[], uint16_t value)
{
    buf[0] = (uint8_t)value;
    buf[1] = value >> 8;
}

static inline uint32_t serialToHost32(const uint8_t buf[])
{
    return (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
}

static inline void hostToSerial32(uint8_t buf[], uint32_t value)
{
    buf[0] = (uint8_t)value;
    buf[1] = value >> 8;
    buf[2] = value >> 16;
    buf[3] = value >> 24;
}

#define hostToSerial(buffer, value) hostToSerialImpl(buffer, &value, sizeof(value))
#define serialToHost(buffer, value) serialToHostImpl(buffer, &value, sizeof(value))


static inline uint8_t *hostToSerialImpl(uint8_t buffer[], const void *value, size_t size)
{
    memcpy(buffer, value, size);
    return buffer + size;
}

static inline const uint8_t *serialToHostImpl(const uint8_t buffer[], void *value, size_t size)
{
    memcpy(value, buffer, size);
    return buffer + size;
}


//if (((void*)&value) == ((void*)(unsigned int)value))

#define hostToSerialArray(buffer, value)                                 \
    {                                                                    \
        for (uint_fast8_t i = 0; i < ARRAY_SIZE(value); i++)             \
        {                                                                \
            buffer = hostToSerialImpl(buffer, value[i], sizeof(*value)); \
        }                                                                \
    }

#define serialToHostArray(buffer, offset, value)                         \
    {                                                                    \
        for (uint_fast8_t i = 0; i < ARRAY_SIZE(value); i++)             \
        {                                                                \
            buffer = serialToHostImpl(buffer, value[i], sizeof(*value)); \
        }                                                                \
    }

#endif /* SERIALIZATION_H */
