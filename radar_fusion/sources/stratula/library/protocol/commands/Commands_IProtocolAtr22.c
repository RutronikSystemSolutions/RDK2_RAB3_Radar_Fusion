/**
 * \file 	Commands_IProtocolAtr22.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IProtocolAtr22 IProtocolAtr22 Commands
 * \brief           Radar Command interface Commands.
 *
 * @{
 */
#include "Commands_IProtocolAtr22.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <universal/components/subinterfaces/iprotocol.h>
#include <universal/protocol/protocol_definitions.h>


uint8_t Commands_IProtocolAtr22_executeWrite(IProtocolAtr22 *protocol, uint16_t wLengthIn, const uint8_t *payload)
{
    if (wLengthIn % sizeof(uint32_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t count         = wLengthIn / sizeof(uint32_t);
    const uint8_t(*commands)[4] = (const uint8_t(*)[4])(uintptr_t)payload;

    return protocol->executeWrite(protocol, commands, count);
}

uint8_t Commands_IProtocolAtr22_executeRead(IProtocolAtr22 *protocol, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != 2 * sizeof(uint16_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t command = serialToHost16(payloadIn);
    const uint16_t count   = serialToHost16(payloadIn + sizeof(command));
    *wLengthOut            = count * sizeof(uint16_t);
    uint16_t *values       = (uint16_t *)(uintptr_t)(*payloadOut);

    return protocol->executeRead(protocol, command, count, values);
}

uint8_t Commands_IProtocolAtr22_setBits(IProtocolAtr22 *protocol, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof(uint32_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t address = serialToHost16(payload);
    const uint16_t bitMask = serialToHost16(payload + sizeof(address));

    return protocol->setBits(protocol, address, bitMask);
}

uint8_t Commands_IProtocolAtr22_read(IProtocolAtr22 *protocol, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IProtocolAtr22_write(IProtocolAtr22 *protocol, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bFunction)
    {
        case FN_PROTOCOL_EXECUTE:
            return Commands_IProtocolAtr22_executeWrite(protocol, wLength, payload);
            break;
        case FN_PROTOCOL_SET_BITS:
            return Commands_IProtocolAtr22_setBits(protocol, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IProtocolAtr22_transfer(IProtocolAtr22 *protocol, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bFunction)
    {
        case FN_PROTOCOL_EXECUTE:
            return Commands_IProtocolAtr22_executeRead(protocol, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
