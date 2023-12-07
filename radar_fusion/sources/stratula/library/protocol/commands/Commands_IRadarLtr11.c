/**
 * \file 	Commands_IRadarLtr11.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRadarLtr11               IRadarLtr11 Commands
 * \brief           Radar Chipset interface Commands.
 *
 * @{
 */
#include "Commands_IRadarLtr11.h"
#include "Commands_IPinsLtr11.h"
#include "Commands_IProtocolLtr11.h"
#include "Commands_IRegisters8_16.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/protocol/protocol_definitions.h>


uint8_t Commands_IRadarLtr11_read(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Ltr11 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_16 *registers = radarLtr11->getIRegisters(radarLtr11);
            return Commands_IRegisters8_16_read(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsLtr11 *pins = radarLtr11->getIPinsLtr11(radarLtr11);
            return Commands_IPinsLtr11_read(pins, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Ltr11 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarLtr11_write(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Ltr11 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_16 *registers = radarLtr11->getIRegisters(radarLtr11);
            return Commands_IRegisters8_16_write(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsLtr11 *pins = radarLtr11->getIPinsLtr11(radarLtr11);
            return Commands_IPinsLtr11_write(pins, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolLtr11 *commands = radarLtr11->getIProtocolLtr11(radarLtr11);
            return Commands_IProtocolLtr11_write(commands, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Ltr11 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarLtr11_transfer(IRadarLtr11 *radarLtr11, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Ltr11 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_16 *registers = radarLtr11->getIRegisters(radarLtr11);
            return Commands_IRegisters8_16_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IProtocolLtr11 *commands = radarLtr11->getIProtocolLtr11(radarLtr11);
            return Commands_IProtocolLtr11_transfer(commands, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolLtr11 *commands = radarLtr11->getIProtocolLtr11(radarLtr11);
            return Commands_IProtocolLtr11_transfer(commands, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Ltr11 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
