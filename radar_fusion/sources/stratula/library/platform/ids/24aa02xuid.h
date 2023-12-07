#ifndef _24AA02XUID_H
#define _24AA02XUID_H 1

#include "getMac.h"
#include "getUuid.h"

sr_t _24aa02xuid_getMac(uint16_t devAddr, uint8_t mac[MAC_LENGTH]);
sr_t _24aa02xuid_getUuid(uint16_t devAddr, uint8_t uuid[UUID_LENGTH]);

#endif /* _24AA02XUID_H */
