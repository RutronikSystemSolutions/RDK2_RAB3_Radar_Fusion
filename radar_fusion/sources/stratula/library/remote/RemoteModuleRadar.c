#include "RemoteModuleRadar.h"

#include <common/serialization.h>
#include <common/type_serialization.h>
#include <common/type_serialization_size.h>
#include <universal/modules/subinterfaces.h>
#include <universal/modules/types.h>
#include <universal/modules/types/imoduleradar.h>
#include <universal/protocol/protocol_definitions.h>

#include <fatal_error.h>
#include <stddef.h>


#define SELF ((RemoteModuleRadar *)(uintptr_t)self)


static void RemoteModuleRadar_registerDataCallback(IModule *self, IProcessing_DataCallback dataCallback, void *arg)
{
    return;
}

static sr_t RemoteModuleRadar_getDataProperties(IModuleRadar *self, IDataProperties_t *info)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteModuleRadar_getRadarInfo(IModuleRadar *self, IProcessingRadarInput_t *info, const IDataProperties_t *dataProperties)
{
    return E_NOT_IMPLEMENTED;
}

static IfxRfe_MmicConfig *RemoteModuleRadar_getConfiguration(IModuleRadar *self)
{
    // E_NOT_IMPLEMENTED;
    return NULL;
}

static IfxRfe_Sequence *RemoteModuleRadar_getSequence(IModuleRadar *self)
{
    // E_NOT_IMPLEMENTED;
    return NULL;
}

static IfxRsp_Stages *RemoteModuleRadar_getProcessingStages(IModuleRadar *self)
{
    // E_NOT_IMPLEMENTED;
    return NULL;
}

static IfxRsp_AntennaCalibration *RemoteModuleRadar_getCalibration(IModuleRadar *self)
{
    // E_NOT_IMPLEMENTED;
    return NULL;
}

static sr_t RemoteModuleRadar_vendorWrite(RemoteModuleRadar *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(MODULE_TYPE_RADAR, MODULE_IMPL_DEFAULT);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, MODULE_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_MODULE, wValue, wIndex, size, buf);
}

static sr_t RemoteModuleRadar_setConfiguration(IModuleRadar *self, const IfxRfe_MmicConfig *c)
{

    const size_t size = sizeof_serialized_IfxRfe_MmicConfig();
    uint8_t buf[size];
    hostToSerial_IfxRfe_MmicConfig(buf, c);

    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_SET_CONFIG, size, buf);
}

static sr_t RemoteModuleRadar_setSequence(IModuleRadar *self, const IfxRfe_Sequence *s)
{
    const size_t size = sizeof_serialized_IfxRfe_Sequence(s->rampCount);
    uint8_t buf[size];
    uint8_t *rampsBuf = hostToSerial_IfxRfe_Sequence(buf, s);
    hostToSerial_IfxRfe_Ramps(rampsBuf, s->ramps, s->rampCount);

    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_SET_SEQUENCE, size, buf);
}

static sr_t RemoteModuleRadar_setProcessingStages(IModuleRadar *self, const IfxRsp_Stages *s)
{
    const size_t size = sizeof_serialized_IfxRsp_Stages();
    uint8_t buf[size];
    hostToSerial_IfxRsp_Stages(buf, s);

    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_SET_PROCESSING_STAGES, size, buf);
}

static sr_t RemoteModuleRadar_setCalibration(IModuleRadar *self, const IfxRsp_AntennaCalibration c[2])
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteModuleRadar_configure(IModule *self)
{
    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_CONFIGURE, 0, NULL);
}

static sr_t RemoteModuleRadar_reset(IModule *self)
{
    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_RESET, 0, NULL);
}

static sr_t RemoteModuleRadar_stopMeasurements(IModule *self)
{
    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_STOP_MEASUREMENT, 0, NULL);
}

static sr_t RemoteModuleRadar_startMeasurements(IModule *self, double measurementCycle)
{
    const size_t size = sizeof(measurementCycle);
    uint8_t buf[size];
    memcpy(buf, &measurementCycle, sizeof(measurementCycle));
    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_START_MEASUREMENT, size, buf);
}

static sr_t RemoteModuleRadar_doMeasurement(IModule *self)
{
    return RemoteModuleRadar_vendorWrite(SELF, FN_MODULE_RADAR_DO_MEASUREMENT, 0, NULL);
}

void RemoteModuleRadar_Constructor(RemoteModuleRadar *self, IVendorCommands *vendorCommands, uint8_t index, uint8_t radarCount, uint8_t radarStartId, uint8_t processingCount, uint8_t processingStartId)
{
    self->m_vendorCommands  = vendorCommands;
    self->m_id              = index;
    self->m_radarCount      = radarCount;
    self->m_processingCount = processingCount;

    if (radarCount > REMOTE_RADAR_MAX_COUNT)
    {
        fatal_error(0);
    }

    for (uint_fast8_t i = 0; i < radarCount; i++)
    {
        //RemoteRadar_Constructor(&self->m_radar[i], vendorCommands, radarStartId + i);
        RemoteRadarRxs_Constructor(&self->m_radarRxs[i], vendorCommands, radarStartId + i);
    }
    for (uint_fast8_t i = 0; i < processingCount; i++)
    {
        RemoteProcessingRadar_Constructor(&self->m_processing[i], vendorCommands, processingStartId + i);
    }

    self->b_IModuleRadar.getDataProperties   = RemoteModuleRadar_getDataProperties;
    self->b_IModuleRadar.getRadarInfo        = RemoteModuleRadar_getRadarInfo;
    self->b_IModuleRadar.getConfiguration    = RemoteModuleRadar_getConfiguration;
    self->b_IModuleRadar.getSequence         = RemoteModuleRadar_getSequence;
    self->b_IModuleRadar.getProcessingStages = RemoteModuleRadar_getProcessingStages;
    self->b_IModuleRadar.getCalibration      = RemoteModuleRadar_getCalibration;

    self->b_IModuleRadar.setConfiguration    = RemoteModuleRadar_setConfiguration;
    self->b_IModuleRadar.setSequence         = RemoteModuleRadar_setSequence;
    self->b_IModuleRadar.setProcessingStages = RemoteModuleRadar_setProcessingStages;
    self->b_IModuleRadar.setCalibration      = RemoteModuleRadar_setCalibration;

    self->b_IModuleRadar.b_IModule.configure            = RemoteModuleRadar_configure;
    self->b_IModuleRadar.b_IModule.reset                = RemoteModuleRadar_reset;
    self->b_IModuleRadar.b_IModule.startMeasurements    = RemoteModuleRadar_startMeasurements;
    self->b_IModuleRadar.b_IModule.stopMeasurements     = RemoteModuleRadar_stopMeasurements;
    self->b_IModuleRadar.b_IModule.doMeasurement        = RemoteModuleRadar_doMeasurement;
    self->b_IModuleRadar.b_IModule.registerDataCallback = RemoteModuleRadar_registerDataCallback;
}
