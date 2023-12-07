/*
 * \file    Commands_IProtocolLtr11.c
 *
 * \addtogroup  Command_Interface   Command Interface
 * \defgroup    Commands_IProtocolLtr11 IProtocolLtr11 Commands
 * \brief       Radar Command interface Commands.
 *
 * @{
 */
#include "Commands_IProtocolLtr11.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <universal/components/subinterfaces/iprotocol.h>
#include <universal/protocol/protocol_definitions.h>

uint8_t Commands_IProtocolLtr11_executeWrite(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload)
{
    if (wLength == 3)
    {
        const uint8_t *command = payload;

        return protocol->executeWrite(protocol, command);
    }
    else if (!(wLength % sizeof(uint16_t)))
    {
        const uint8_t *command    = payload;
        const uint16_t offset     = 2 * sizeof(uint8_t);
        const uint16_t count      = (wLength - offset) / sizeof(uint16_t);
        const uint8_t(*values)[2] = (const uint8_t(*)[2])(uintptr_t)(payload + offset);

        return protocol->executeWriteBurst(protocol, command, count, values);
    }
    else
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
}

uint8_t Commands_IProtocolLtr11_executeRead(IProtocolLtr11 *protocol, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn == sizeof(uint8_t))
    {
        uint16_t *value       = (uint16_t *)(uintptr_t)(*payloadOut);
        *wLengthOut           = sizeof(*value);
        const uint8_t command = serialToHost8(payloadIn);

        return protocol->executeRead(protocol, command, value);
    }
    else if (wLengthIn == (2 * sizeof(uint8_t)) + sizeof(uint16_t))
    {
        const uint8_t *command = payloadIn;
        const uint16_t offset  = 2 * sizeof(uint8_t);
        const uint16_t count   = serialToHost16(payloadIn + offset);
        *wLengthOut            = count * sizeof(uint16_t);
        uint16_t *values       = (uint16_t *)(uintptr_t)(*payloadOut);

        return protocol->executeReadBurst(protocol, command, count, values);
    }
    else
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
}

uint8_t Commands_IProtocolLtr11_setBits(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != (sizeof(uint8_t) + sizeof(uint16_t)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t address  = payload[0];
    const uint16_t bitMask = serialToHost16(payload + sizeof(address));

    return protocol->setBits(protocol, address, bitMask);
}

uint8_t Commands_IProtocolLtr11_setting(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload)
{
    if (wLength == sizeof(uint16_t))
    {
        const uint16_t prt = serialToHost16(payload);

        return protocol->setMisoArbitration(protocol, prt);
    }
    else
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
}

uint8_t Commands_IProtocolLtr11_executeWriteBatch(IProtocolLtr11 *protocol, uint16_t wLength, const uint8_t *payload)
{
    if (wLength % (3 * sizeof(uint8_t)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t(*commands)[3] = (const uint8_t(*)[3])(uintptr_t)payload;
    const uint16_t count        = wLength / (3 * sizeof(uint8_t));

    return protocol->executeWriteBatch(protocol, commands, count);
}

uint8_t Commands_IProtocolLtr11_read(IProtocolLtr11 *protocol, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IProtocolLtr11_write(IProtocolLtr11 *protocol, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bFunction)
    {
        case FN_PROTOCOL_EXECUTE:
            return Commands_IProtocolLtr11_executeWrite(protocol, wLength, payload);
            break;
        case FN_PROTOCOL_SET_BITS:
            return Commands_IProtocolLtr11_setBits(protocol, wLength, payload);
            break;
        case FN_PROTOCOL_SETTING:
            return Commands_IProtocolLtr11_setting(protocol, wLength, payload);
            break;
        case FN_PROTOCOL_EXECUTE_HELPER:
            return Commands_IProtocolLtr11_executeWriteBatch(protocol, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IProtocolLtr11_transfer(IProtocolLtr11 *protocol, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bFunction)
    {
        case FN_PROTOCOL_EXECUTE:
            return Commands_IProtocolLtr11_executeRead(protocol, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
