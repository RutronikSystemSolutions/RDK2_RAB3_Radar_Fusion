#ifndef COMMANDS_I_REGISTERS_8_32_H
#define COMMANDS_I_REGISTERS_8_32_H 1

#include <components/interfaces/IRegisters8_32.h>


uint8_t Commands_IRegisters8_32_read(IRegisters8_32 *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRegisters8_32_write(IRegisters8_32 *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRegisters8_32_transfer(IRegisters8_32 *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* COMMANDS_I_REGISTERS_8_32_H */
