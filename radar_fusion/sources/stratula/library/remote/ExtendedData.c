#include "ExtendedData.h"

#include <common/type_serialization.h>
#include <common/type_serialization_size.h>
#include <stddef.h>
#include <string.h>
#include <universal/protocol/protocol_definitions.h>

/******************************************************************************/
/*Interface Methods Declaration ----------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Interface Definition -------------------------------------------------------*/
/******************************************************************************/
IData ExtendedData = {
    .calibrationRequired = ExtendedData_calibrationRequired,
    .calibrate           = ExtendedData_calibrate,
    .configure           = ExtendedData_configure,
    .start               = ExtendedData_start,
    .stop                = ExtendedData_stop,
    .getStatusFlags      = ExtendedData_getStatusFlags,
};

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
static IVendorCommands *m_vendorCommands;
static IData *m_localData;
static uint8_t m_localCount = 0;


/******************************************************************************/
/*Interface Methods Definition -----------------------------------------------*/
/******************************************************************************/
void ExtendedData_Constructor(IVendorCommands *vendorCommands, IData *localData, uint8_t localCount)
{
    m_localData      = localData;
    m_localCount     = localCount;
    m_vendorCommands = vendorCommands;
}

sr_t ExtendedData_calibrate(uint8_t index)
{
    if (index < m_localCount)
    {
        return m_localData->calibrate(index);
    }

    //Remote data-index
    index = index - m_localCount;
    return m_vendorCommands->vendorWrite(REQ_DATA, REQ_DATA_CALIBRATE,
                                         index,
                                         0,
                                         NULL);
}

sr_t ExtendedData_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize)
{
    if (index < m_localCount)
    {
        return m_localData->configure(index, dataProperties, settings, settingsSize);
    }

    //Remote data-index
    index                 = index - m_localCount;
    const uint16_t length = sizeof_serialized_IDataProperties() + settingsSize;
    uint8_t buf[length];
    hostToSerial_IDataProperties(buf, dataProperties);
    if (settings == NULL)
    {
        if (settingsSize != 0)
        {
            return E_INVALID_SIZE;
        }
    }
    else
    {
        memcpy(buf + sizeof_serialized_IDataProperties(), settings, settingsSize);
    }

    return m_vendorCommands->vendorWrite(REQ_DATA, REQ_DATA_CONFIGURE,
                                         index,
                                         length,
                                         buf);
}

sr_t ExtendedData_start(uint8_t index)
{
    if (index < m_localCount)
    {
        return m_localData->start(index);
    }

    //Remote data-index
    index = index - m_localCount;
    return m_vendorCommands->vendorWrite(REQ_DATA, REQ_DATA_START,
                                         index,
                                         0,
                                         NULL);
}

sr_t ExtendedData_stop(uint8_t index)
{
    if (index < m_localCount)
    {
        return m_localData->stop(index);
    }

    //Remote data-index
    index = index - m_localCount;
    return m_vendorCommands->vendorWrite(REQ_DATA, REQ_DATA_STOP,
                                         index,
                                         0,
                                         NULL);
}

sr_t ExtendedData_getStatusFlags(uint8_t index, uint32_t *flags)
{
    if (index < m_localCount)
    {
        return m_localData->getStatusFlags(index, flags);
    }

    //Remote data-index
    index             = index - m_localCount;
    uint8_t **payload = (uint8_t **)&flags;
    sr_t result       = m_vendorCommands->vendorRead(REQ_DATA,
                                                     REQ_DATA_STATUS_FLAGS,
                                                     index,
                                                     sizeof(*flags),
                                                     payload);
    *flags            = **payload;
    return result;
}

sr_t ExtendedData_calibrationRequired(uint8_t index, double dataRate, bool *isRequired)
{
    *isRequired = false;
    return E_NOT_IMPLEMENTED;
}
