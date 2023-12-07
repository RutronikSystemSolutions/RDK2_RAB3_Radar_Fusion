/**
* \file 	Commands_IRadar.c
*
* \addtogroup      Command_Interface   Command Interface
*
* \defgroup        Commands_IRadar               IRadar Commands
* \brief           Radar Sensor interface Commands.
*
* @{
*/
#include "Commands_IRadar.h"
#include "Commands_IRadarAtr22.h"
#include "Commands_IRadarAvian.h"
#include "Commands_IRadarLtr11.h"
#include "Commands_IRadarRxs.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <protocol/ProtocolHandler.h>
#include <universal/components/implementations/radar.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/components/types/iradar.h>
#include <universal/implementations.h>
#include <universal/protocol/protocol_definitions.h>


static IRadar *m_radar[MAX_INSTANCE_REGISTRATIONS];  //Radar instance registrations
static uint8_t m_radarCount;                         //Number of currently registered radar components


static inline uint8_t Commands_IRadar_getImplementation(IRadar *radar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(radar->m_bImplementation))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    *payload[0] = radar->m_bImplementation;

    return E_SUCCESS;
}

uint8_t Commands_IRadar_initialize(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return radar->initialize(radar);
}

uint8_t Commands_IRadar_configure(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof_serialized_IfxRfe_MmicConfig())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IfxRfe_MmicConfig config;
    serialToHost_IfxRfe_MmicConfig(payload, &config);
    return radar->configure(radar, &config);
}

uint8_t Commands_IRadar_reset(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    if (payload[0] > 1)
    {
        return STATUS_REQUEST_WINDEX_INVALID;
    }
    const bool softReset = payload[0];
    return radar->reset(radar, softReset);
}

uint8_t Commands_IRadar_loadSequence(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    // unserialize sequence and obtain rampCount
    if (wLength < sizeof_serialized_IfxRfe_Sequence(0))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    IfxRfe_Sequence sequence;
    const uint8_t *rampsBuf = serialToHost_IfxRfe_Sequence(payload, &sequence);
    if (wLength != sizeof_serialized_IfxRfe_Sequence(sequence.rampCount))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    // unserialize ramps
    IfxRfe_Ramp ramps[sequence.rampCount];
    if (sequence.rampCount > MAX_RAMPS_SUPPORTED)
    {
        return STATUS_COMMAND_PAYLOAD_INVALID;
    }
    serialToHost_IfxRfe_Ramps(rampsBuf, ramps, sequence.rampCount);

    sequence.ramps = ramps;
    return radar->loadSequence(radar, &sequence);
}

uint8_t Commands_IRadar_calibrate(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return radar->calibrate(radar);
}

uint8_t Commands_IRadar_startSequence(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return radar->startSequence(radar);
}

uint8_t Commands_IRadar_startData(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    ProtocolHandler_initializeSendingDataFrames();

    return radar->startData(radar);
}

uint8_t Commands_IRadar_stopData(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return radar->stopData(radar);
}

uint8_t Commands_IRadar_getDataIndex(IRadar *radar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    uint8_t dataIndex;
    sr_t result = radar->getDataIndex(radar, &dataIndex);
    *payload[0] = dataIndex;

    return result;
}

uint8_t Commands_IRadar_getRxChannels(IRadar *radar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    uint8_t rxChannels;
    sr_t result = radar->getRxChannels(radar, &rxChannels);
    *payload[0] = rxChannels;

    return result;
}

uint8_t Commands_IRadar_enableConstantFrequencyMode(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    float txPower;
    if (wLength != (sizeof(uint16_t) + sizeof(txPower)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t txMask = serialToHost16(payload);
    serialToHost(payload + sizeof(uint16_t), txPower);
    return radar->enableConstantFrequencyMode(radar, txMask, txPower);
}

uint8_t Commands_IRadar_setConstantFrequency(IRadar *radar, uint16_t wLength, const uint8_t *payload)
{
    double frequency;
    if (wLength != sizeof(frequency))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    serialToHost(payload, frequency);
    return radar->setConstantFrequency(radar, frequency);
}

static uint8_t checkCommandTarget(uint8_t bImplementation, uint8_t bId)
{
    if (bId >= m_radarCount)
    {
        return STATUS_COMMAND_ID_INVALID;
    }
    IRadar *radar = m_radar[bId];
    if (radar == NULL)
    {
        return STATUS_COMMAND_ID_INVALID;
    }

    if ((bImplementation != COMPONENT_IMPL_DEFAULT) && (bImplementation != radar->m_bImplementation))
    {
        // The implementation type has to fit the registered component's type
        // But each special implementation can also be treated as default implementation as it inherits the default functionality.
        return STATUS_COMMAND_IMPL_INVALID;
    }
    return STATUS_SUCCESS;
}

static inline uint8_t Commands_IRadar_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IRadar *radar = m_radar[bId];

    switch (bImplementation)
    {
        case COMPONENT_IMPL_DEFAULT:
            break;  //execute general Radar functions below
        case COMPONENT_IMPL_RADAR_RXS:
            return Commands_IRadarRxs_read((IRadarRxs *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_AVIAN:
            return Commands_IRadarAvian_read((IRadarAvian *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_ATR22:
            return Commands_IRadarAtr22_read((IRadarAtr22 *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_LTR11:
            return Commands_IRadarLtr11_read((IRadarLtr11 *)radar, bSubinterface, bFunction, wLength, payload);
        default:
            return STATUS_COMMAND_IMPL_INVALID;  // not implemented
    }

    //This must be checked after the implementation check since some implementations can have subinterfaces
    if (bSubinterface != COMPONENT_SUBIF_DEFAULT)
    {
        //no special sub interface supported for default implementation
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    // General Radar functions
    switch (bFunction)
    {
        case FN_GET_IMPL:
            return Commands_IRadar_getImplementation(radar, wLength, payload);
            break;
        case FN_RADAR_GET_DATA_INDEX:
            return Commands_IRadar_getDataIndex(radar, wLength, payload);
            break;
        case FN_RADAR_GET_RX_CHANNELS:
            return Commands_IRadar_getRxChannels(radar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IRadar_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IRadar *radar = m_radar[bId];

    switch (bImplementation)
    {
        case COMPONENT_IMPL_DEFAULT:
            break;  //execute general Radar functions below
        case COMPONENT_IMPL_RADAR_RXS:
            return Commands_IRadarRxs_write((IRadarRxs *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_AVIAN:
            return Commands_IRadarAvian_write((IRadarAvian *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_ATR22:
            return Commands_IRadarAtr22_write((IRadarAtr22 *)radar, bSubinterface, bFunction, wLength, payload);
        case COMPONENT_IMPL_RADAR_LTR11:
            return Commands_IRadarLtr11_write((IRadarLtr11 *)radar, bSubinterface, bFunction, wLength, payload);
        default:
            return STATUS_COMMAND_IMPL_INVALID;  // not implemented
    }

    //This must be checked after the implementation check since some implementations can have subinterfaces
    if (bSubinterface != COMPONENT_SUBIF_DEFAULT)
    {
        //no special sub interface supported for default implementation
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    // General Radar functions
    switch (bFunction)
    {
        case FN_RADAR_INITIALIZE:
            return Commands_IRadar_initialize(radar, wLength, payload);
            break;
        case FN_RADAR_RESET:
            return Commands_IRadar_reset(radar, wLength, payload);
            break;
        case FN_RADAR_CONFIGURE:
            return Commands_IRadar_configure(radar, wLength, payload);
            break;
        case FN_RADAR_CALIBRATE:
            return Commands_IRadar_calibrate(radar, wLength, payload);
            break;
        case FN_RADAR_LOAD_SEQUENCE:
            return Commands_IRadar_loadSequence(radar, wLength, payload);
            break;
        case FN_RADAR_START_SEQUENCE:
            return Commands_IRadar_startSequence(radar, wLength, payload);
            break;
        case FN_RADAR_START_DATA:
            return Commands_IRadar_startData(radar, wLength, payload);
            break;
        case FN_RADAR_STOP_DATA:
            return Commands_IRadar_stopData(radar, wLength, payload);
            break;
        case FN_RADAR_ENABLE_CONSTANT_FREQ:
            return Commands_IRadar_enableConstantFrequencyMode(radar, wLength, payload);
            break;
        case FN_RADAR_SET_CONSTANT_FREQ:
            return Commands_IRadar_setConstantFrequency(radar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IRadar_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IRadar *radar = m_radar[bId];

    switch (bImplementation)
    {
        case COMPONENT_IMPL_DEFAULT:
            break;  //execute general Radar functions below
        case COMPONENT_IMPL_RADAR_RXS:
            return Commands_IRadarRxs_transfer((IRadarRxs *)radar, bSubinterface, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        case COMPONENT_IMPL_RADAR_AVIAN:
            return Commands_IRadarAvian_transfer((IRadarAvian *)radar, bSubinterface, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        case COMPONENT_IMPL_RADAR_ATR22:
            return Commands_IRadarAtr22_transfer((IRadarAtr22 *)radar, bSubinterface, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        case COMPONENT_IMPL_RADAR_LTR11:
            return Commands_IRadarLtr11_transfer((IRadarLtr11 *)radar, bSubinterface, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
        default:
            return STATUS_COMMAND_IMPL_INVALID;  // not implemented
    }

    //This must be checked after the implementation check since some implementations can have subinterfaces
    if (bSubinterface != COMPONENT_SUBIF_DEFAULT)
    {
        //no special sub interface supported for default implementation
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    // General Radar functions
    // There are currently no transfer functions in this component
    return STATUS_COMMAND_FUNCTION_INVALID;
}

void Commands_IRadar_Constructor(ICommands *self)
{
    m_radarCount   = 0;
    self->m_bType  = COMPONENT_TYPE_RADAR;
    self->read     = Commands_IRadar_read;
    self->write    = Commands_IRadar_write;
    self->transfer = Commands_IRadar_transfer;
}

bool Commands_IRadar_register(IRadar *radar)
{
    if (m_radarCount >= MAX_INSTANCE_REGISTRATIONS)
    {
        return false;
    }
    m_radar[m_radarCount] = radar;
    m_radarCount++;
    return true;
}

/*  @} */
