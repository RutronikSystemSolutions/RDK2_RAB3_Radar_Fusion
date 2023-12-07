#include "getUuidFromMac.h"

#include "getMac.h"
#include <string.h>


sr_t getUuidFromMac(uint8_t uuid[UUID_LENGTH])
{
    memset(uuid + MAC_LENGTH, 0, UUID_LENGTH - MAC_LENGTH);

    return getMac(uuid);
}
