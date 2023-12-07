/**
 * \file 	Requests_I2c.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_I2c               I2C Commands
 * \brief           I2C bus related Commands.
 *
 * @{
 */
#include "Requests_II2c.h"
#include <universal/protocol/protocol_definitions.h>

#include <stdbool.h>


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
uint8_t Requests_II2c_write(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (!(wIndex & REQ_I2C_FLAG_TRANSACTION))
    {
        if (wLength != 0)
        {
            return STATUS_REQUEST_WLENGTH_INVALID;
        }
        const uint16_t busId = wValue & 0xF000;
        const uint32_t speed = ((wValue & 0x0FFF) << 15) | wIndex;

        if (speed == 0)
        {
            return i2c->clearBus(busId);
        }
        else
        {
            return i2c->configureBusSpeed(busId, speed);
        }
    }
    else
    {
        if (wIndex & REQ_I2C_FLAG_CONFIGURE_DEFAULT)
        {
            return STATUS_REQUEST_NOT_IMPLEMENTED;
        }
        else if (wIndex & REQ_I2C_FLAG_ACK_POLLING)
        {
            if (wLength != 0)
            {
                return STATUS_REQUEST_WLENGTH_INVALID;
            }

            const uint16_t devAddr = wValue;
            return i2c->pollForAck(devAddr);
        }
        else
        {
            const uint16_t devAddr = wValue;
            const bool withRegAddr = wIndex & REQ_I2C_FLAG_WITH_REG_ADDR;
            const uint16_t length  = wLength;

            if (withRegAddr)
            {
                const uint8_t prefix = (uint8_t)wIndex;
                return i2c->writeWith8BitPrefix(devAddr, prefix, length, payload);
            }
            else
            {
                return i2c->writeWithoutPrefix(devAddr, length, payload);
            }
        }
    }
}

uint8_t Requests_II2c_transaction16_write(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    const uint16_t devAddr = wValue;
    const uint16_t prefix  = wIndex;
    const uint16_t length  = wLength;
    return i2c->writeWith16BitPrefix(devAddr, prefix, length, payload);
}

uint8_t Requests_II2c_read(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (!(wIndex & REQ_I2C_FLAG_TRANSACTION))
    {
        return STATUS_REQUEST_WINDEX_INVALID;
    }
    else
    {
        if (wIndex & REQ_I2C_FLAG_CONFIGURE_DEFAULT)
        {
            return STATUS_REQUEST_NOT_IMPLEMENTED;
        }
        else
        {
            const uint16_t devAddr = wValue;
            const bool withRegAddr = wIndex & REQ_I2C_FLAG_WITH_REG_ADDR;
            const uint16_t length  = wLength;

            if (withRegAddr)
            {
                const uint8_t prefix = (uint8_t)wIndex;
                return i2c->readWith8BitPrefix(devAddr, prefix, length, *payload);
            }
            else
            {
                return i2c->readWithoutPrefix(devAddr, length, *payload);
            }
        }
    }
}

uint8_t Requests_II2c_transaction16_read(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    const uint16_t devAddr = wValue;
    const uint16_t prefix  = wIndex;
    const uint16_t length  = wLength;
    return i2c->readWith16BitPrefix(devAddr, prefix, length, *payload);
}


/*  @} */
