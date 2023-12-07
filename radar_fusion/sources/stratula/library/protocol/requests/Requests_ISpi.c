/**
 * \file 	Requests_Spi.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_Spi               SPI Commands
 * \brief           SPI related Commands.
 *
 * @{
 */
#include "Requests_ISpi.h"
#include <stdbool.h>
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
uint8_t Requests_ISpi_write(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (!(wIndex & REQ_SPI_FLAG_TRANSACTION))
    {
        if (wLength != sizeof(uint32_t))
        {
            return STATUS_REQUEST_WLENGTH_INVALID;
        }

        const uint8_t devId    = wValue & 0xFF;
        const uint8_t wordSize = wValue >> 8;
        const uint8_t flags    = wIndex & 0xFF;
        const uint32_t speed   = ((uint32_t *)(uintptr_t)payload)[0];

        return spi->configure(devId, flags, wordSize, speed);
    }

    const uint8_t devId       = wValue & 0xFF;
    const uint8_t bufferWidth = wValue >> 8;
    const bool keepSel        = wIndex & REQ_SPI_FLAG_KEEP_SELECTED;

    switch (bufferWidth)
    {
        case 1:
            return spi->write8(devId, wLength / 1, payload, keepSel);
            break;
        case 2:
            return spi->write16(devId, wLength / 2, (const uint16_t *)(uintptr_t)payload, keepSel);
            break;
        case 4:
            return spi->write32(devId, wLength / 4, (const uint32_t *)(uintptr_t)payload, keepSel);
            break;
        default:
            return STATUS_REQUEST_WVALUE_INVALID;
    }
}

uint8_t Requests_ISpi_read(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    const uint8_t devId       = wValue & 0xFF;
    const uint8_t bufferWidth = wValue >> 8;
    const bool keepSel        = wIndex & REQ_SPI_FLAG_KEEP_SELECTED;

    switch (bufferWidth)
    {
        case 1:
            return spi->read8(devId, wLength / 1, *payload, keepSel);
            break;
        case 2:
            return spi->read16(devId, wLength / 2, (uint16_t *)(uintptr_t)(*payload), keepSel);
            break;
        case 4:
            return spi->read32(devId, wLength / 4, (uint32_t *)(uintptr_t)(*payload), keepSel);
            break;
        default:
            return STATUS_REQUEST_WVALUE_INVALID;
    }
}

uint8_t Requests_ISpi_transfer(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t devId       = wValue & 0xFF;
    const uint8_t bufferWidth = wValue >> 8;
    const bool keepSel        = wIndex & REQ_SPI_FLAG_KEEP_SELECTED;
    *wLengthOut               = wLengthIn;

    switch (bufferWidth)
    {
        case 1:
            return spi->transfer8(devId, wLengthIn / 1, payloadIn, *payloadOut, keepSel);
            break;
        case 2:
            return spi->transfer16(devId, wLengthIn / 2, (const uint16_t *)(uintptr_t)payloadIn, (uint16_t *)(uintptr_t)(*payloadOut), keepSel);
            break;
        case 4:
            return spi->transfer32(devId, wLengthIn / 4, (const uint32_t *)(uintptr_t)payloadIn, (uint32_t *)(uintptr_t)(*payloadOut), keepSel);
            break;
        default:
            return STATUS_REQUEST_WVALUE_INVALID;
            break;
    }
}

/*  @} */
