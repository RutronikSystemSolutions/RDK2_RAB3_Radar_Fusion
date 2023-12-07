#include "RemoteProcessingRadar.h"

#include <common/serialization.h>
#include <common/type_serialization.h>
#include <common/type_serialization_size.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/components/types/iprocessingradar.h>
#include <universal/protocol/protocol_definitions.h>

#define SELF ((RemoteProcessingRadar *)(uintptr_t)self)

static sr_t RemoteProcessingRadar_vendorRead(RemoteProcessingRadar *self, uint8_t bFunction, uint16_t size, uint8_t **buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_PROCESSING_RADAR, COMPONENT_IMPL_DEFAULT);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorRead(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteProcessingRadar_vendorWrite(RemoteProcessingRadar *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_PROCESSING_RADAR, COMPONENT_IMPL_DEFAULT);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteProcessingRadar_reinitialize(IProcessingRadar *self)
{
    return RemoteProcessingRadar_vendorWrite(SELF, FN_PROCESSING_RADAR_REINIT, 0, NULL);
}

static sr_t RemoteProcessingRadar_start(IProcessingRadar *self)
{
    return RemoteProcessingRadar_vendorWrite(SELF, FN_PROCESSING_RADAR_START, 0, NULL);
}

static bool RemoteProcessingRadar_isBusy(IProcessingRadar *self)
{
    uint8_t buf[1];
    uint8_t *payload = buf;

    const sr_t result = RemoteProcessingRadar_vendorRead(SELF, FN_PROCESSING_RADAR_IS_BUSY, 1, &payload);

    if (result != E_SUCCESS)
    {
        return true;
    }

    return (*payload != 0);
}

static uint32_t RemoteProcessingRadar_allocateCommonMemory(IProcessingRadar *self, uint32_t size)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteProcessingRadar_writeConfigRam(IProcessingRadar *self, uint16_t offset, uint16_t count, const uint32_t *ramContent)
{
    //No configuration shall be written while it is busy
    const bool busy = RemoteProcessingRadar_isBusy(self);
    if (busy)
    {
        RETURN_ON_ERROR(RemoteProcessingRadar_reinitialize(self));
    }

    const uint32_t argSize     = sizeof(offset);
    const uint16_t maxDataSize = SELF->m_vendorCommands->getMaxTransfer() - argSize;
    uint32_t dataSize          = count * sizeof(ramContent[0]);

    while (dataSize > 0)
    {
        uint16_t wLength = (dataSize > maxDataSize) ? maxDataSize : dataSize;
        uint16_t wCount  = wLength / sizeof(ramContent[0]);
        uint8_t buf[wLength + argSize];
        memcpy(buf, ramContent, wLength);
        hostToSerial16(buf + wLength, offset);

        RETURN_ON_ERROR(RemoteProcessingRadar_vendorWrite(SELF, FN_PROCESSING_RADAR_WRITE_CONFIG_RAM, wLength + argSize, buf));

        offset += wCount;
        ramContent += wCount;
        dataSize -= wLength;
    }
    return E_SUCCESS;
}

static sr_t RemoteProcessingRadar_configure(IProcessingRadar *self, uint8_t dataSource, const IDataProperties_t *dataProperties, const IProcessingRadarInput_t *radarInfo,
                                            const IfxRsp_Stages *stages, const IfxRsp_AntennaCalibration *antennaConfig)
{
    const size_t size = sizeof(dataSource) + sizeof_serialized_IDataProperties() + sizeof_serialized_IProcessingRadarInput() + sizeof_serialized_IfxRsp_Stages() + 2 * sizeof_serialized_IfxRsp_AntennaCalibration();
    uint8_t buf[size];

    uint8_t *offset = buf;
    offset          = hostToSerial(offset, dataSource);
    offset          = hostToSerial_IDataProperties(offset, dataProperties);
    offset          = hostToSerial_IProcessingRadarInput(offset, radarInfo);
    offset          = hostToSerial_IfxRsp_Stages(offset, stages);
    if (antennaConfig != NULL)
    {
        offset = hostToSerial_IfxRsp_AntennaCalibration(offset, &antennaConfig[0]);
        offset = hostToSerial_IfxRsp_AntennaCalibration(offset, &antennaConfig[1]);
    }

    return RemoteProcessingRadar_vendorWrite(SELF, FN_PROCESSING_RADAR_CONFIGURE, size, buf);
}

static sr_t RemoteProcessingRadar_writeCustomWindowCoefficients(IProcessingRadar *self, uint8_t slotNr, uint16_t offset, uint16_t count, const uint32_t *coefficients)
{
    const uint16_t argSize     = sizeof(slotNr) + sizeof(offset);
    const uint16_t maxDataSize = SELF->m_vendorCommands->getMaxTransfer() - argSize;
    uint32_t dataSize          = count * sizeof(coefficients[0]);

    while (dataSize > 0)
    {
        uint16_t wLength = (dataSize > maxDataSize) ? maxDataSize : dataSize;
        uint16_t wCount  = wLength / sizeof(coefficients[0]);
        uint8_t buf[wLength + argSize];
        memcpy(buf, coefficients, wLength);
        hostToSerial16(buf + wLength, offset);
        hostToSerial8(buf + wLength + sizeof(offset), slotNr);

        RETURN_ON_ERROR(RemoteProcessingRadar_vendorWrite(SELF, FN_PROCESSING_RADAR_WRITE_CUSTOM_WINDOW_COEFFICIENTS, wLength + argSize, buf));

        offset += wCount;
        coefficients += wCount;
        dataSize -= wLength;
    }
    return E_SUCCESS;
}

static sr_t RemoteProcessingRadar_getRawData(IProcessingRadar *self, IfxRsp_Signal **data)
{
    const size_t size = sizeof_serialized_IfxRsp_Signal();
    uint8_t buf[size];
    uint8_t *payload = buf;

    const sr_t result = RemoteProcessingRadar_vendorRead(SELF, FN_PROCESSING_RADAR_GET_RAW_DATA, size, &payload);

    serialToHost_IfxRsp_Signal(buf, &SELF->m_rawData);
    *data = &SELF->m_rawData;
    return result;
}

void RemoteProcessingRadar_Constructor(RemoteProcessingRadar *self, IVendorCommands *vendorCommands, uint8_t id)
{
    self->b_IProcessingRadar.m_bImplementation = COMPONENT_IMPL_DEFAULT;

    self->m_vendorCommands = vendorCommands;
    self->m_id             = id;

    self->b_IProcessingRadar.reinitialize = RemoteProcessingRadar_reinitialize;
    self->b_IProcessingRadar.configure    = RemoteProcessingRadar_configure;
    self->b_IProcessingRadar.start        = RemoteProcessingRadar_start;

    self->b_IProcessingRadar.writeCustomWindowCoefficients = RemoteProcessingRadar_writeCustomWindowCoefficients;
    self->b_IProcessingRadar.writeConfigRam                = RemoteProcessingRadar_writeConfigRam;
    self->b_IProcessingRadar.isBusy                        = RemoteProcessingRadar_isBusy;
    self->b_IProcessingRadar.allocateCommonMemory          = RemoteProcessingRadar_allocateCommonMemory;

    self->b_IProcessingRadar.getRawData = RemoteProcessingRadar_getRawData;
}
