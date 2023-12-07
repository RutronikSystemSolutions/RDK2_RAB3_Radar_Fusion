#ifndef COMMANDS_IRADAR_ATR22_H_
#define COMMANDS_IRADAR_ATR22_H_ 1

#include <components/interfaces/IRadarAtr22.h>
#include <stdint.h>

uint8_t Commands_IRadarAtr22_read(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRadarAtr22_write(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRadarAtr22_transfer(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_IRADAR_ATR22_H_ */
