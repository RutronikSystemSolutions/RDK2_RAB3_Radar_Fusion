#include "24aa02xuid.h"

#include <impl/PlatformI2c.h>

#define LOCATION_48_BIT_ID  0xFA
#define LOCATION_128_BIT_ID 0xF0


sr_t _24aa02xuid_getMac(uint16_t devAddr, uint8_t mac[MAC_LENGTH])
{
    return PlatformI2c_readWith8BitPrefix(devAddr, LOCATION_48_BIT_ID, MAC_LENGTH, mac);
}

sr_t _24aa02xuid_getUuid(uint16_t devAddr, uint8_t uuid[UUID_LENGTH])
{
    return PlatformI2c_readWith8BitPrefix(devAddr, LOCATION_128_BIT_ID, UUID_LENGTH, uuid);
}
