#include "../getMac.h"
#include "../getUuid.h"

#include "../24aa02xuid.h"
#include "../24cw128x.h"
//#include "../getUuidFromMac.h"


#include <BoardDefinition.h>


// the following definitions are selected by including this header once in the project
// the linker will complain if multiple implementations are included


sr_t getUuid(uint8_t uuid[UUID_LENGTH])
{
#if defined(BOARD_EEPROM_ADDRESS_24AA02XUID)
    return _24aa02xuid_getUuid(BOARD_EEPROM_ADDRESS_24AA02XUID, uuid);
#elif defined(BOARD_EEPROM_ADDRESS_24CW128X)
    return _24cw128x_getUuid(BOARD_EEPROM_ADDRESS_24CW128X, uuid);
//#elseif
//    return getUuidFromMac(uuid);
#else
    return E_NOT_IMPLEMENTED;
#endif
}

sr_t getMac(uint8_t mac[MAC_LENGTH])
{
#if defined(BOARD_EEPROM_ADDRESS_24AA02XUID)
    return _24aa02xuid_getMac(BOARD_EEPROM_ADDRESS_24AA02XUID, mac);
#else
    return E_NOT_IMPLEMENTED;
#endif
}
