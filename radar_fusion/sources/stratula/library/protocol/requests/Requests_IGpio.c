/**
 * \file 	Requests_Gpio.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_Gpio               GPIO Commands
 * \brief           General Purpose I/O related Commands.
 *
 * @{
 */
#include "Requests_IGpio.h"
#include <common/errors.h>
#include <universal/protocol/protocol_definitions.h>

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/* Public Methods Definition -------------------------------------------------*/
/******************************************************************************/
uint8_t Requests_IGpio_read(IGpio *gpio, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != 1)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    bool state;
    sr_t result = gpio->getPin(wIndex, &state);
    *payload[0] = (uint8_t)state;
    return result;
}

uint8_t Requests_IGpio_write(IGpio *gpio, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint16_t id = wIndex;
    if (wValue > 1)
    {
        const uint8_t flags = wValue >> 8;
        return gpio->configurePin(id, flags);
    }
    else
    {
        const bool state = wValue;
        return gpio->setPin(id, state);
    }
}

/*  @} */
