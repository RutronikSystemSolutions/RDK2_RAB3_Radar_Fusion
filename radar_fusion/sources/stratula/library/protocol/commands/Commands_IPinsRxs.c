/**
 * \file 	Commands_IRadar.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRadar               IRadar Commands
 * \brief           Radar Sensor interface Commands.
 *
 * @{
 */
#include "Commands_IPinsRxs.h"
#include <common/errors.h>
#include <universal/components/subinterfaces/ipins.h>
#include <universal/protocol/protocol_definitions.h>

uint8_t Commands_IPinsRxs_setResetPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 1)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    if (payload[0] > 1)
    {
        return STATUS_COMMAND_PAYLOAD_INVALID;
    }

    const bool state = (payload[0] != 0);
    return pinsRadar->setResetPin(pinsRadar, state);
}

uint8_t Commands_IPinsRxs_reset(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return pinsRadar->reset(pinsRadar);
}

uint8_t Commands_IPinsRxs_configureDmuxPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 2)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t index = payload[0];
    const uint8_t flags = payload[1];
    return pinsRadar->configureDmuxPin(pinsRadar, index, flags);
}

uint8_t Commands_IPinsRxs_setDmuxPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 2)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t index = payload[0];
    const bool state    = (payload[1] != 0);
    return pinsRadar->setDmuxPin(pinsRadar, index, state);
}


uint8_t Commands_IPinsRxs_getOkPin(IPinsRxs *pinsRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != 1)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    bool state;
    sr_t result = pinsRadar->getOkPin(pinsRadar, &state);
    *payload[0] = (uint8_t)state;

    return result;
}

uint8_t Commands_IPinsRxs_getDmuxPin(IPinsRxs *pinsRadar, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != 1)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t index = payloadIn[0];
    bool state;
    sr_t result    = pinsRadar->getDmuxPin(pinsRadar, index, &state);
    *payloadOut[0] = (uint8_t)state;
    *wLengthOut    = 1;

    return result;
}

uint8_t Commands_IPinsRxs_read(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bFunction)
    {
        case FN_PINS_GET_OK:
            return Commands_IPinsRxs_getOkPin(pinsRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsRxs_write(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bFunction)
    {
        case FN_PINS_SET_RESET_PIN:
            return Commands_IPinsRxs_setResetPin(pinsRadar, wLength, payload);
            break;
        case FN_PINS_RESET:
            return Commands_IPinsRxs_reset(pinsRadar, wLength, payload);
            break;
        case FN_PINS_CONFIGURE_DMUX:
            return Commands_IPinsRxs_configureDmuxPin(pinsRadar, wLength, payload);
            break;
        case FN_PINS_SET_DMUX:
            return Commands_IPinsRxs_setDmuxPin(pinsRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsRxs_transfer(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bFunction)
    {
        case FN_PINS_GET_DMUX:
            return Commands_IPinsRxs_getDmuxPin(pinsRadar, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
