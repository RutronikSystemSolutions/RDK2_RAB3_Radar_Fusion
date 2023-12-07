/**
 * \file 	Commands_IRadarAtr22.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRadarAtr22               IRadarAtr22 Commands
 * \brief           Radar Chipset interface Commands.
 *
 * @{
 */
#include "Commands_IRadarAtr22.h"
#include "Commands_IProtocolAtr22.h"
#include "Commands_IRegisters16.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/protocol/protocol_definitions.h>


uint8_t Commands_IRadarAtr22_read(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Atr22 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarAtr22->getIRegisters(radarAtr22);
            return Commands_IRegisters16_read(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolAtr22 *commands = radarAtr22->getIProtocolAtr22(radarAtr22);
            return Commands_IProtocolAtr22_read(commands, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Atr22 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarAtr22_write(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Atr22 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarAtr22->getIRegisters(radarAtr22);
            return Commands_IRegisters16_write(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolAtr22 *commands = radarAtr22->getIProtocolAtr22(radarAtr22);
            return Commands_IProtocolAtr22_write(commands, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Atr22 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarAtr22_transfer(IRadarAtr22 *radarAtr22, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute Atr22 functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarAtr22->getIRegisters(radarAtr22);
            return Commands_IRegisters16_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PINS:
        {
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
        }
        case COMPONENT_SUBIF_PROTOCOL:
        {
            IProtocolAtr22 *commands = radarAtr22->getIProtocolAtr22(radarAtr22);
            return Commands_IProtocolAtr22_transfer(commands, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // Atr22 functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
