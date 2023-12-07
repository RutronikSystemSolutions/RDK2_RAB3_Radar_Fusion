#ifndef COMMANDS_I_REGISTERS_8_H
#define COMMANDS_I_REGISTERS_8_H 1

#include <components/interfaces/IRegisters8.h>


uint8_t Commands_IRegisters8_read(IRegisters8 *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRegisters8_write(IRegisters8 *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRegisters8_transfer(IRegisters8 *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* COMMANDS_I_REGISTERS_8_H */
