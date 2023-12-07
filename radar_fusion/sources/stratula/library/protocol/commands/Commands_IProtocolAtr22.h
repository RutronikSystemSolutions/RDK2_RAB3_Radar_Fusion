#ifndef COMMANDS_I_COMMANDS_ATR22_H
#define COMMANDS_I_COMMANDS_ATR22_H 1

#include <components/interfaces/IProtocolAtr22.h>
#include <stdint.h>


uint8_t Commands_IProtocolAtr22_executeWrite(IProtocolAtr22 *protocol, uint16_t wLengthIn, const uint8_t *payload);
uint8_t Commands_IProtocolAtr22_executeRead(IProtocolAtr22 *protocol, uint16_t wLength, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);
uint8_t Commands_IProtocolAtr22_setBits(IProtocolAtr22 *protocol, uint16_t wLength, const uint8_t *payload);

uint8_t Commands_IProtocolAtr22_read(IProtocolAtr22 *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IProtocolAtr22_write(IProtocolAtr22 *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolAtr22_transfer(IProtocolAtr22 *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_I_COMMANDS_ATR22_H */
