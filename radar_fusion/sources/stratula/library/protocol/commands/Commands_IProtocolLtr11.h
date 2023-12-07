#ifndef COMMANDS_ICOMMANDSLTR11_H_
#define COMMANDS_ICOMMANDSLTR11_H_ 1

#include <components/interfaces/IProtocolLtr11.h>
#include <stdint.h>

uint8_t Commands_IProtocolLtr11_executeWrite(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolLtr11_executeRead(IProtocolLtr11 *protocol, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);
uint8_t Commands_IProtocolLtr11_setBits(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolLtr11_setting(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolLtr11_executeWriteBatch(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload);

uint8_t Commands_IProtocolLtr11_read(IProtocolLtr11 *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IProtocolLtr11_write(IProtocolLtr11 *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProtocolLtr11_transfer(IProtocolLtr11 *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_ICOMMANDSLTR11_H_ */
