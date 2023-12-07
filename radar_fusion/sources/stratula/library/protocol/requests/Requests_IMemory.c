/**
 * \file 	Requests_Memory.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_Memory               Memory Commands
 * \brief           Memory related Commands.
 *
 * @{
 */
#include "Requests_IMemory.h"

#include <common/errors.h>
#include <impl/PlatformMemory.h>
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
uint8_t Requests_IMemory_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    const uint32_t address = ((wIndex << 16) | wValue);

    return PlatformMemory_read(address, wLength, payload);
}

uint8_t Requests_IMemory_write(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    const uint32_t address = ((wIndex << 16) | wValue);

    return PlatformMemory_write(address, wLength, payload);
}

uint8_t Requests_IMemory_stream(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != (sizeof(uint32_t) * 2))
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint32_t dstAddr = ((wIndex << 16) | wValue);
    const uint32_t srcAddr = ((uint32_t *)(uintptr_t)payload)[0];
    const uint32_t length  = ((uint32_t *)(uintptr_t)payload)[1];

    return PlatformMemory_stream(dstAddr, srcAddr, length);
}

/*  @} */
