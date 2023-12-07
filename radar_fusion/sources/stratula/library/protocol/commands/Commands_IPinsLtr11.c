/**
* \file 	Commands_IPinsLtr11.c
*
* \addtogroup      Command_Interface   Command Interface
*
* \defgroup        Commands_IPinsLtr11               IPinsLtr11 Commands
* \brief           Radar Sensor pins interface Commands.
*
* @{
*/
#include "Commands_IPinsLtr11.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <universal/components/subinterfaces/ipins.h>
#include <universal/protocol/protocol_definitions.h>

uint8_t Commands_IPinsLtr11_setResetPin(IPinsLtr11 *pinsLtr11, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    if (payload[0] > 1)  // bool
    {
        return STATUS_COMMAND_PAYLOAD_INVALID;
    }

    const bool state = (payload[0] != 0);
    return pinsLtr11->setResetPin(pinsLtr11, state);
}

uint8_t Commands_IPinsLtr11_getIrqPin(IPinsLtr11 *pinsLtr11, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    bool state;
    sr_t result = pinsLtr11->getIrqPin(pinsLtr11, &state);
    *payload[0] = (uint8_t)state;

    return result;
}

uint8_t Commands_IPinsLtr11_getDetectionPins(IPinsLtr11 *pinsLtr11, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    uint8_t states;
    sr_t result = pinsLtr11->getDetectionPins(pinsLtr11, &states);
    *payload[0] = states;

    return result;
}

uint8_t Commands_IPinsLtr11_reset(IPinsLtr11 *pinsLtr11, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return pinsLtr11->reset(pinsLtr11);
}

uint8_t Commands_IPinsLtr11_read(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    if (pinsLtr11 == NULL)
    {
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    switch (bFunction)
    {
        case FN_PINS_GET_IRQ:
            return Commands_IPinsLtr11_getIrqPin(pinsLtr11, wLength, payload);
            break;
        case FN_PINS_GET_DETECTIONS:
            return Commands_IPinsLtr11_getDetectionPins(pinsLtr11, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsLtr11_write(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    if (pinsLtr11 == NULL)
    {
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    switch (bFunction)
    {
        case FN_PINS_SET_RESET_PIN:
            return Commands_IPinsLtr11_setResetPin(pinsLtr11, wLength, payload);
            break;
        case FN_PINS_RESET:
            return Commands_IPinsLtr11_reset(pinsLtr11, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsLtr11_transfer(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (pinsLtr11 == NULL)
    {
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
