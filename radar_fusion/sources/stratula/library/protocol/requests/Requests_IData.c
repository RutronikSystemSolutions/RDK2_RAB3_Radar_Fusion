/**
 * \file 	Requests_IData.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_IData               IData Commands
 * \brief           Data interface Commands.
 *
 * @{
 */
#include "Requests_IData.h"
#include <common/errors.h>
#include <common/type_serialization.h>
#include <protocol/ProtocolHandler.h>
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
static inline uint8_t Requests_IData_start(IData *data, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    ProtocolHandler_initializeSendingDataFrames();

    const uint8_t id = (uint8_t)wIndex;

    return data->start(id);
}

static inline uint8_t Requests_IData_stop(IData *data, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint8_t id = (uint8_t)wIndex;

    return data->stop(id);
}

static inline uint8_t Requests_IData_calibrate(IData *data, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint8_t id = (uint8_t)wIndex;

    return data->calibrate(id);
}

static inline uint8_t Requests_IData_getStatusFlags(IData *data, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint32_t))
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint8_t id = (uint8_t)wIndex;

    return data->getStatusFlags(id, (uint32_t *)(uintptr_t)(*payload));
}

static inline uint8_t Requests_IData_configure(IData *data, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    if (wLength < sizeof_serialized_IDataProperties())
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    const uint8_t id = (uint8_t)wIndex;

    IDataProperties_t dataProperties;
    serialToHost_IDataProperties(payload, &dataProperties);

    // remaining settings are passed as a buffer
    const uint8_t *settings = payload + sizeof_serialized_IDataProperties();
    const uint16_t length   = wLength - sizeof_serialized_IDataProperties();

    return data->configure(id, &dataProperties, settings, length);
}

/******************************************************************************/
/* Public Methods Definition -------------------------------------------------*/
/******************************************************************************/

uint8_t Requests_IData_write(IData *data, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    switch (wValue)
    {
        case REQ_DATA_CONFIGURE:
            return Requests_IData_configure(data, wIndex, wLength, payload);
            break;
        case REQ_DATA_CALIBRATE:
            return Requests_IData_calibrate(data, wIndex, wLength, payload);
            break;
        case REQ_DATA_START:
            return Requests_IData_start(data, wIndex, wLength, payload);
            break;
        case REQ_DATA_STOP:
            return Requests_IData_stop(data, wIndex, wLength, payload);
            break;
        default:
            return STATUS_REQUEST_WVALUE_INVALID;
            break;
    }
}

uint8_t Requests_IData_read(IData *data, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    switch (wValue)
    {
        case REQ_DATA_STATUS_FLAGS:
            return Requests_IData_getStatusFlags(data, wIndex, wLength, payload);
            break;
        default:
            return STATUS_REQUEST_WVALUE_INVALID;
            break;
    }
}


/*  @} */
