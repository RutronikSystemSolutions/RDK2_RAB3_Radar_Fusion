/**
 * \file 	Commands_IRadarRxs.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRadarRxs               IRadarRxs Commands
 * \brief           Radar Chipset interface Commands.
 *
 * @{
 */
#include "Commands_IRadarRxs.h"
#include "Commands_IPinsRxs.h"
#include "Commands_IRegisters16.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <stdbool.h>
#include <universal/components/implementations/radar.h>
#include <universal/components/implementations/radar/iradarrxs.h>
#include <universal/components/subinterfaces.h>
#include <universal/protocol/protocol_definitions.h>


uint8_t Commands_IRadarRxs_startFirmwareFunction(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload)
{
    if ((wLength & 1) || (wLength < sizeof(uint16_t)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t callCode   = serialToHost16(payload);
    const uint16_t paramCount = (wLength - sizeof(callCode)) / sizeof(uint16_t);
    const uint16_t *params    = (const uint16_t *)(uintptr_t)(payload + sizeof(callCode));
    return radarRxs->startFirmwareFunction(radarRxs, callCode, params, paramCount);
}

uint8_t Commands_IRadarRxs_configureDmuxMap(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != I_PINS_RXS_DMUX_COUNT)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t *map = payload;
    return radarRxs->configureDmuxMap(radarRxs, map);
}

uint8_t Commands_IRadarRxs_setDividerOutput(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 1)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    if (payload[0] > 1)
    {
        return STATUS_REQUEST_WINDEX_INVALID;
    }
    const bool enable = payload[0];
    return radarRxs->enableDividerOutput(radarRxs, enable);
}

uint8_t Commands_IRadarRxs_checkFirmwareFunctionStatus(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload)
{
    if (wLength != (2 * sizeof(uint16_t)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    uint16_t *status = (uint16_t *)(uintptr_t)(*payload);
    uint16_t *count  = (uint16_t *)(uintptr_t)(*payload + sizeof(uint16_t));
    *count           = 0;  // initialize return value, since we need to return a count in case of an error
    radarRxs->checkFirmwareFunctionStatus(radarRxs, status, count);
    return E_SUCCESS;  // always send the return values, even if the function failed
}

uint8_t Commands_IRadarRxs_getFirmwareFunctionResult(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload)
{
    if (wLength & 1)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t retCount = wLength / sizeof(uint16_t);
    uint16_t *retVals       = (uint16_t *)(uintptr_t)(*payload);
    return radarRxs->getFirmwareFunctionResult(radarRxs, retVals, retCount);
}

uint8_t Commands_IRadarRxs_getStatus(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    bool status;
    sr_t result = radarRxs->getStatus(radarRxs, &status);
    *payload[0] = (uint8_t)status;

    return result;
}

uint8_t Commands_IRadarRxs_setDataCrc(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    if (payload[0] > 1)
    {
        return STATUS_REQUEST_WINDEX_INVALID;
    }
    const bool enable = payload[0];
    return radarRxs->enableDataCrc(radarRxs, enable);
}

uint8_t Commands_IRadarRxs_setTriggerSource(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t src = payload[0];
    return radarRxs->setTriggerSource(radarRxs, src);
}

uint8_t Commands_IRadarRxs_read(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute RXS functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarRxs->getIRegisters(radarRxs);
            return Commands_IRegisters16_read(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsRxs *pins = radarRxs->getIPinsRxs(radarRxs);
            return Commands_IPinsRxs_read(pins, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // RXS functions
    switch (bFunction)
    {
        case FN_RADAR_RXS_GET_STATUS:
            return Commands_IRadarRxs_getStatus(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_CHECK_FW_FUNCTION_STATUS:
            return Commands_IRadarRxs_checkFirmwareFunctionStatus(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_GET_FW_FUNCTION_RESULT:
            return Commands_IRadarRxs_getFirmwareFunctionResult(radarRxs, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarRxs_write(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute RXS functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarRxs->getIRegisters(radarRxs);
            return Commands_IRegisters16_write(registers, bFunction, wLength, payload);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsRxs *pins = radarRxs->getIPinsRxs(radarRxs);
            return Commands_IPinsRxs_write(pins, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // RXS functions
    switch (bFunction)
    {
        case FN_RADAR_RXS_START_FW_FUNCTION:
            return Commands_IRadarRxs_startFirmwareFunction(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_SET_DIV_OUTPUT:
            return Commands_IRadarRxs_setDividerOutput(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_SET_DATA_CRC:
            return Commands_IRadarRxs_setDataCrc(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_SET_TRIGGER_SRC:
            return Commands_IRadarRxs_setTriggerSource(radarRxs, wLength, payload);
            break;
        case FN_RADAR_RXS_CONFIGURE_DMUX_MAP:
            return Commands_IRadarRxs_configureDmuxMap(radarRxs, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IRadarRxs_transfer(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            break;  //execute RXS functions below
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = radarRxs->getIRegisters(radarRxs);
            return Commands_IRegisters16_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        case COMPONENT_SUBIF_PINS:
        {
            IPinsRxs *pins = radarRxs->getIPinsRxs(radarRxs);
            return Commands_IPinsRxs_transfer(pins, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;  // not implemented
    }

    // RXS functions
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
