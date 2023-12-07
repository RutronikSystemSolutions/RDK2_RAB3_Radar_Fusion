#ifndef COMMANDS_I_REGISTERS_16_H
#define COMMANDS_I_REGISTERS_16_H 1

#include <components/interfaces/IRegisters16.h>


uint8_t Commands_IRegisters16_read(IRegisters16 *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRegisters16_write(IRegisters16 *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRegisters16_transfer(IRegisters16 *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* COMMANDS_I_REGISTERS_16_H */
