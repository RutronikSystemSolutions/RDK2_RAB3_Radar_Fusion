#ifndef COMMANDSIRADARLTR11_H_
#define COMMANDSIRADARLTR11_H_ 1

#include <components/interfaces/IRadarLtr11.h>
#include <stdint.h>

uint8_t Commands_IRadarLtr11_read(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRadarLtr11_write(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRadarLtr11_transfer(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDSIRADARLTR11_H_ */