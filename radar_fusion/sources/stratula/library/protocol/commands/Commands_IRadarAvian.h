#ifndef COMMANDS_IRADAR_AVIAN_H_
#define COMMANDS_IRADAR_AVIAN_H_ 1

#include <components/interfaces/IRadarAvian.h>
#include <stdint.h>

uint8_t Commands_IRadarAvian_read(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRadarAvian_write(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRadarAvian_transfer(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_IRADAR_AVIAN_H_ */
