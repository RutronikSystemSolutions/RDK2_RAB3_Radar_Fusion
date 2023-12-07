/**
 * \file 	Commands_IRadarAvian.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRadarAvian               IRadarAvian Commands
 * \brief           Radar Chipset interface Commands.
 *
 * @{
 */
#include "Commands_IRadarAvian.h"
#include "Commands_IPinsAvian.h"
#include "Commands_IProtocolAvian.h"
#include "Commands_IRegisters8_32.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/protocol/protocol_definitions.h>


uint8_t Commands_IRadarAvian_read(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Avian functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_32 *registers = radarAvian->getIRegisters(radarAvian);
            return Commands_IRegisters8_32_read(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsAvian *pins = radarAvian->getIPinsAvian(radarAvian);
            return Commands_IPinsAvian_read(pins, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Avian functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarAvian_write(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Avian functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_32 *registers = radarAvian->getIRegisters(radarAvian);
            return Commands_IRegisters8_32_write(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsAvian *pins = radarAvian->getIPinsAvian(radarAvian);
            return Commands_IPinsAvian_write(pins, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolAvian *commands = radarAvian->getIProtocolAvian(radarAvian);
            return Commands_IProtocolAvian_write(commands, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Avian functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarAvian_transfer(IRadarAvian *radarAvian, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Avian functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8_32 *registers = radarAvian->getIRegisters(radarAvian);
            return Commands_IRegisters8_32_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsAvian *pins = radarAvian->getIPinsAvian(radarAvian);
            return Commands_IPinsAvian_transfer(pins, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolAvian *commands = radarAvian->getIProtocolAvian(radarAvian);
            return Commands_IProtocolAvian_transfer(commands, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Avian functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
