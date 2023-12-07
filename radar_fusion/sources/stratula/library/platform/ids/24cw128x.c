#include "24cw128x.h"

#include <impl/PlatformI2c.h>
#include <string.h>


#define LOCATION_MAGIC      0x0000
#define LOCATION_128_BIT_ID 0x0010

const uint8_t magic[8] = {'I', 'f', 'x', 'B', 'o', 'a', 'r', 'd'};


sr_t _24cw128x_getUuid(uint16_t devAddr, uint8_t uuid[UUID_LENGTH])
{
    uint8_t buf[8];
    RETURN_ON_ERROR(PlatformI2c_readWith16BitPrefix(devAddr, LOCATION_MAGIC, sizeof(buf), buf));

    if (memcmp(buf, magic, sizeof(magic)) != 0)
    {
        return E_UNEXPECTED_VALUE;
    }

    return PlatformI2c_readWith16BitPrefix(devAddr, LOCATION_128_BIT_ID, UUID_LENGTH, uuid);
}
