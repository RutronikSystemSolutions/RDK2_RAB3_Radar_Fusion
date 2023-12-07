#ifndef COMMANDS_I_COMMANDS_AVIAN_H
#define COMMANDS_I_COMMANDS_AVIAN_H 1

#include <components/interfaces/IProtocolAvian.h>
#include <stdint.h>


uint8_t Commands_IProtocolAvian_execute(IProtocolAvian *protocol, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);
uint8_t Commands_IProtocolAvian_executeWrite(IProtocolAvian *protocol, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolAvian_setBits(IProtocolAvian *protocol, uint16_t wLength, const uint8_t *payload);


uint8_t Commands_IProtocolAvian_read(IProtocolAvian *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IProtocolAvian_write(IProtocolAvian *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolAvian_transfer(IProtocolAvian *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_I_COMMANDS_AVIAN_H */
