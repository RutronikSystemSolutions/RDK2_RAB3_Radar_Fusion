/**
 * \file 	Commands_IPinsAvian.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IPinsAvian               IPinsAvian Commands
 * \brief           Radar Sensor pins interface Commands.
 *
 * @{
 */
#include "Commands_IPinsAvian.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <universal/components/subinterfaces/ipins.h>
#include <universal/protocol/protocol_definitions.h>

uint8_t Commands_IPinsAvian_setResetPin(IPinsAvian *pinsAvian, uint16_t wLength, const uint8_t *payload)
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
    return pinsAvian->setResetPin(pinsAvian, state);
}

uint8_t Commands_IPinsAvian_getIrqPin(IPinsAvian *pinsAvian, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    bool state;
    sr_t result = pinsAvian->getIrqPin(pinsAvian, &state);
    *payload[0] = (uint8_t)state;

    return result;
}

uint8_t Commands_IPinsAvian_reset(IPinsAvian *pinsAvian, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return pinsAvian->reset(pinsAvian);
}

uint8_t Commands_IPinsAvian_read(IPinsAvian *pinsAvian, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    if (pinsAvian == NULL)
    {
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    switch (bFunction)
    {
        case FN_PINS_GET_IRQ:
            return Commands_IPinsAvian_getIrqPin(pinsAvian, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsAvian_write(IPinsAvian *pinsAvian, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    if (pinsAvian == NULL)
    {
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    switch (bFunction)
    {
        case FN_PINS_SET_RESET_PIN:
            return Commands_IPinsAvian_setResetPin(pinsAvian, wLength, payload);
            break;
        case FN_PINS_RESET:
            return Commands_IPinsAvian_reset(pinsAvian, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t Commands_IPinsAvian_transfer(IPinsAvian *pinsAvian, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (pinsAvian == NULL)
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
