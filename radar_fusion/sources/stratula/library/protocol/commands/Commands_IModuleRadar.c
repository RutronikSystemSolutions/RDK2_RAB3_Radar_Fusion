/**
 * \file    Commands_IModuleRadar.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IModuleRadar               IModuleRadar Commands
 * \brief           Module Radar interface Commands.
 *
 * @{
 */
#include "Commands_IModuleRadar.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <modules/radar/ModuleRadar.h>
#include <protocol/ProtocolHandler.h>
#include <universal/modules/subinterfaces.h>
#include <universal/modules/types.h>
#include <universal/modules/types/imoduleradar.h>
#include <universal/protocol/protocol_definitions.h>


static IModuleRadar *m_moduleRadar[MAX_INSTANCE_REGISTRATIONS];
static uint8_t m_moduleRadarCount;


uint8_t Commands_IModuleRadar_startMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    double updateCycle;

    if (wLength != sizeof(updateCycle))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    ProtocolHandler_initializeSendingDataFrames();

    IModule *module = (IModule *)moduleRadar;
    serialToHost(payload, updateCycle);

    const sr_t result = module->startMeasurements(module, updateCycle);
    return result;
}

uint8_t Commands_IModuleRadar_stopMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IModule *module = (IModule *)moduleRadar;
    return module->stopMeasurements(module);
}

uint8_t Commands_IModuleRadar_doMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IModule *module = (IModule *)moduleRadar;
    return module->doMeasurement(module);
}

uint8_t Commands_IModuleRadar_reset(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IModule *module = (IModule *)moduleRadar;
    return module->reset(module);
}

uint8_t Commands_IModuleRadar_configure(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IModule *module = (IModule *)moduleRadar;
    return module->configure(module);
}

uint8_t Commands_IModuleRadar_setConfiguration(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof_serialized_IfxRfe_MmicConfig())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    serialToHost_IfxRfe_MmicConfig(payload, moduleRadar->getConfiguration(moduleRadar));

    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_setSequence(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    // unserialize sequence and obtain rampCount
    if (wLength < sizeof_serialized_IfxRfe_Sequence(0))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IfxRfe_Sequence *s      = moduleRadar->getSequence(moduleRadar);
    const uint8_t *rampsBuf = serialToHost_IfxRfe_Sequence(payload, s);

    if (wLength != sizeof_serialized_IfxRfe_Sequence(s->rampCount))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    // unserialize ramps
    if (s->rampCount > MAX_RAMPS_SUPPORTED)
    {
        return STATUS_COMMAND_PAYLOAD_INVALID;
    }

    serialToHost_IfxRfe_Ramps(rampsBuf, s->ramps, s->rampCount);

    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_setProcessingStages(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof_serialized_IfxRsp_Stages())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    serialToHost_IfxRsp_Stages(payload, moduleRadar->getProcessingStages(moduleRadar));

    return E_SUCCESS;
}

uint8_t Commands_IModuleRadar_setCalibration(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 2 * sizeof_serialized_IfxRsp_AntennaCalibration())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IfxRsp_AntennaCalibration *c = moduleRadar->getCalibration(moduleRadar);
    const uint8_t *offset        = serialToHost_IfxRsp_AntennaCalibration(payload, &c[0]);
    serialToHost_IfxRsp_AntennaCalibration(offset, &c[1]);

    return E_SUCCESS;
}

uint8_t Commands_IModuleRadar_getConfiguration(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IfxRfe_MmicConfig())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    hostToSerial_IfxRfe_MmicConfig(*payload, moduleRadar->getConfiguration(moduleRadar));

    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_getSequence(IModuleRadar *moduleRadar, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    IfxRfe_Sequence *s = moduleRadar->getSequence(moduleRadar);
    *wLengthOut        = sizeof_serialized_IfxRfe_Sequence(s->rampCount);

    uint8_t *rampOffset = hostToSerial_IfxRfe_Sequence(*payloadOut, s);
    hostToSerial_IfxRfe_Ramps(rampOffset, s->ramps, s->rampCount);

    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_getDataProperties(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IDataProperties())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    IDataProperties_t dataProperties;
    moduleRadar->getDataProperties(moduleRadar, &dataProperties);
    hostToSerial_IDataProperties(*payload, &dataProperties);
    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_getRadarInfo(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IProcessingRadarInput())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IDataProperties_t dataProperties;
    moduleRadar->getDataProperties(moduleRadar, &dataProperties);

    IProcessingRadarInput_t radarInfo;
    moduleRadar->getRadarInfo(moduleRadar, &radarInfo, &dataProperties);
    hostToSerial_IProcessingRadarInput(*payload, &radarInfo);

    return STATUS_SUCCESS;
}

uint8_t Commands_IModuleRadar_getProcessingStages(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IfxRsp_Stages())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    hostToSerial_IfxRsp_Stages(*payload, moduleRadar->getProcessingStages(moduleRadar));

    return E_SUCCESS;
}

uint8_t Commands_IModuleRadar_getCalibration(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != 2 * sizeof_serialized_IfxRsp_AntennaCalibration())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IfxRsp_AntennaCalibration *c = moduleRadar->getCalibration(moduleRadar);

    uint8_t *offset = hostToSerial_IfxRsp_AntennaCalibration(*payload, &c[0]);
    hostToSerial_IfxRsp_AntennaCalibration(offset, &c[1]);

    return E_SUCCESS;
}

static inline uint8_t checkCommandTarget(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface)
{
    if (bId >= m_moduleRadarCount)
    {
        return STATUS_COMMAND_ID_INVALID;
    }
    if (m_moduleRadar[bId] == NULL)
    {
        return STATUS_COMMAND_ID_INVALID;
    }

    if (bImplementation != MODULE_IMPL_DEFAULT)
    {
        //No special implementations available
        return STATUS_COMMAND_IMPL_INVALID;
    }

    if (bSubinterface != MODULE_SUBIF_DEFAULT)
    {
        //No subinterface implementations available
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    return STATUS_SUCCESS;
}

static inline uint8_t Commands_IModuleRadar_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IModuleRadar *moduleRadar = m_moduleRadar[bId];

    switch (bFunction)
    {
        case FN_MODULE_RADAR_GET_CONFIG:
            return Commands_IModuleRadar_getConfiguration(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_GET_DATA_PROPERTIES:
            return Commands_IModuleRadar_getDataProperties(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_GET_RADAR_INFO:
            return Commands_IModuleRadar_getRadarInfo(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_GET_PROCESSING_STAGES:
            return Commands_IModuleRadar_getProcessingStages(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_GET_CALIBRATION:
            return Commands_IModuleRadar_getCalibration(moduleRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IModuleRadar_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IModuleRadar *moduleRadar = m_moduleRadar[bId];

    switch (bFunction)
    {
        case FN_MODULE_RADAR_SET_CONFIG:
            return Commands_IModuleRadar_setConfiguration(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_SET_SEQUENCE:
            return Commands_IModuleRadar_setSequence(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_SET_PROCESSING_STAGES:
            return Commands_IModuleRadar_setProcessingStages(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_RESET:
            return Commands_IModuleRadar_reset(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_CONFIGURE:
            return Commands_IModuleRadar_configure(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_START_MEASUREMENT:
            return Commands_IModuleRadar_startMeasurement(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_STOP_MEASUREMENT:
            return Commands_IModuleRadar_stopMeasurement(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_DO_MEASUREMENT:
            return Commands_IModuleRadar_doMeasurement(moduleRadar, wLength, payload);
            break;
        case FN_MODULE_RADAR_SET_CALIBRATION:
            return Commands_IModuleRadar_setCalibration(moduleRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IModuleRadar_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IModuleRadar *moduleRadar = m_moduleRadar[bId];

    switch (bFunction)
    {
        case FN_MODULE_RADAR_GET_SEQUENCE:
            return Commands_IModuleRadar_getSequence(moduleRadar, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

void Commands_IModuleRadar_Constructor(ICommands *self)
{
    m_moduleRadarCount = 0;
    self->m_bType      = MODULE_TYPE_RADAR;
    self->read         = Commands_IModuleRadar_read;
    self->write        = Commands_IModuleRadar_write;
    self->transfer     = Commands_IModuleRadar_transfer;
}

bool Commands_IModuleRadar_registerModule(IModuleRadar *moduleRadar)
{
    if (m_moduleRadarCount >= MAX_INSTANCE_REGISTRATIONS)
    {
        return false;
    }
    m_moduleRadar[m_moduleRadarCount] = moduleRadar;
    m_moduleRadarCount++;
    return true;
}

/*  @} */
