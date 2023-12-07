#include "crc16.h"

//todo: move to default platform implementation
#ifndef TARGET_PLATFORM_CRC_ENGINE

uint16_t crc16_ccitt_false(const uint8_t buf[], uint16_t len, uint16_t crc)
{
    while (len--)
    {
        uint8_t x = (crc >> 8) ^ *buf++;
        x ^= x >> 4;
        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
    }
    return crc;
}

uint16_t crc16_ccitt_false_reg(uint32_t val)
{
    uint16_t crc = CRC16_CCITT_FALSE_SEED;

    for (int_fast8_t s = 24; s >= 0; s -= 8)
    {
        uint8_t x = (crc >> 8) ^ (uint8_t)(val >> s);
        x ^= x >> 4;
        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
    }
    return crc;
}

#endif
