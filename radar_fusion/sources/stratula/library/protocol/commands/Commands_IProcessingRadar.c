/**
 * \file 	Commands_IProcessingRadar.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IProcessingRadar               IProcessingRadar Commands
 * \brief           Radar Processing interface Commands.
 *
 * @{
 */
#include "Commands_IProcessingRadar.h"
#include <common/errors.h>
#include <common/serialization.h>
#include <common/type_serialization.h>
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/components/types/iprocessingradar.h>
#include <universal/implementations.h>
#include <universal/protocol/protocol_definitions.h>

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
static IfxRsp_Signal m_lastOperationResult;
static IProcessingRadar *m_processing[MAX_INSTANCE_REGISTRATIONS];  //Processing instance registrations
static uint8_t m_processingCount;                                   //Number of currently registered radar components

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

static inline uint8_t Commands_IProcessingRadar_getImplementation(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(processingRadar->m_bImplementation))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    *payload[0] = processingRadar->m_bImplementation;

    return E_SUCCESS;
}

/******************************************************************************/
/* Public Methods Definition -------------------------------------------------*/
/******************************************************************************/
uint8_t Commands_IProcessingRadar_doFft(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    IfxRsp_Signal input;
    IfxRsp_FftSetting fftSettings;
    uint16_t samples, sample_offset;
    uint8_t dimension, format;

    const uint16_t expectedLength = sizeof_serialized_IfxRsp_Signal() + sizeof_serialized_IfxRsp_FftSetting() + sizeof(samples) + sizeof(sample_offset) + sizeof(dimension) + sizeof(format);
    if (expectedLength != wLength)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    payload = serialToHost_IfxRsp_Signal(payload, &input);
    payload = serialToHost_IfxRsp_FftSetting(payload, &fftSettings);

    payload = serialToHost(payload, samples);
    payload = serialToHost(payload, sample_offset);
    payload = serialToHost(payload, dimension);
    payload = serialToHost(payload, format);

    return processingRadar->doFft(processingRadar, &input, &fftSettings, &m_lastOperationResult, samples, sample_offset, dimension, format);
}

uint8_t Commands_IProcessingRadar_doNci(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    IfxRsp_Signal input;
    uint8_t outputFormat;

    const uint16_t expectedLength = sizeof_serialized_IfxRsp_Signal() + sizeof(outputFormat);
    if (expectedLength != wLength)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    payload = serialToHost_IfxRsp_Signal(payload, &input);
    payload = serialToHost(payload, outputFormat);

    return processingRadar->doNci(processingRadar, &input, outputFormat, &m_lastOperationResult);
}

uint8_t Commands_IProcessingRadar_doPsd(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    IfxRsp_Signal input;
    uint16_t nFft;

    const uint16_t expectedLength = sizeof_serialized_IfxRsp_Signal() + sizeof(nFft);
    if (expectedLength != wLength)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    payload = serialToHost_IfxRsp_Signal(payload, &input);
    payload = serialToHost(payload, nFft);

    return processingRadar->doPsd(processingRadar, &input, nFft, &m_lastOperationResult);
}

uint8_t Commands_IProcessingRadar_doThresholding(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    IfxRsp_Signal input;
    uint8_t dimension;
    IfxRsp_ThresholdingSetting settings;

    const uint16_t expectedLength = sizeof_serialized_IfxRsp_Signal() + sizeof(dimension) +
                                    sizeof_serialized_IfxRsp_ThresholdingSetting();
    if (expectedLength != wLength)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    payload = serialToHost_IfxRsp_Signal(payload, &input);
    payload = serialToHost(payload, dimension);
    payload = serialToHost_IfxRsp_ThresholdingSetting(payload, &settings);

    return processingRadar->doThresholding(processingRadar, &input, dimension, &settings, &m_lastOperationResult);
}

uint8_t Commands_IProcessingRadar_configure(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    uint8_t dataSource;
    IDataProperties_t dataProperties;
    IProcessingRadarInput_t radarInfo;
    IfxRsp_Stages stages;
    IfxRsp_AntennaCalibration calibration[2];

    const uint16_t expectedLength = sizeof(dataSource) + sizeof_serialized_IDataProperties() + sizeof_serialized_IProcessingRadarInput() + sizeof_serialized_IfxRsp_Stages();
    const uint16_t optionalLength = expectedLength + 2 * sizeof_serialized_IfxRsp_AntennaCalibration();
    bool optional                 = false;
    if (wLength == optionalLength)
    {
        optional = true;
    }
    else if (wLength != expectedLength)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    payload = serialToHost(payload, dataSource);
    payload = serialToHost_IDataProperties(payload, &dataProperties);
    payload = serialToHost_IProcessingRadarInput(payload, &radarInfo);
    payload = serialToHost_IfxRsp_Stages(payload, &stages);
    if (optional)
    {
        payload = serialToHost_IfxRsp_AntennaCalibration(payload, &calibration[0]);
        serialToHost_IfxRsp_AntennaCalibration(payload, &calibration[1]);
        return processingRadar->configure(processingRadar, dataSource, &dataProperties, &radarInfo, &stages, calibration);
    }
    else
    {
        return processingRadar->configure(processingRadar, dataSource, &dataProperties, &radarInfo, &stages, NULL);
    }
}

uint8_t Commands_IProcessingRadar_getRawData(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IfxRsp_Signal())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    IfxRsp_Signal *rawData;
    RETURN_ON_ERROR(processingRadar->getRawData(processingRadar, &rawData));
    hostToSerial_IfxRsp_Signal(*payload, rawData);
    return STATUS_SUCCESS;
}

uint8_t Commands_IProcessingRadar_readOperationResult(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof_serialized_IfxRsp_Signal())
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    hostToSerial_IfxRsp_Signal(*payload, &m_lastOperationResult);
    return STATUS_SUCCESS;
}

uint8_t Commands_IProcessingRadar_writeConfigRam(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    const uint16_t argSize = sizeof(uint16_t);
    const uint16_t length  = wLength - argSize;  //pure length of the values part without additional parameters
    //wLength (without offset) shall be a multiple of 4
    if (length & 0x0003)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t count   = length / 4;
    const uint32_t *values = (uint32_t *)(uintptr_t)payload;
    const uint16_t offset  = serialToHost16(payload + length);

    return processingRadar->writeConfigRam(processingRadar, offset, count, values);
}

uint8_t Commands_IProcessingRadar_writeCustomWindowCoefficients(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    const uint16_t argSize = sizeof(uint16_t) + sizeof(uint8_t);
    const uint16_t length  = wLength - argSize;  //pure length of the coefficients part without additional parameters
    //wLength shall be a multiple of 4
    if (length & 0x0003)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t count         = length / sizeof(uint32_t);
    const uint32_t *coefficients = (uint32_t *)(uintptr_t)payload;
    const uint16_t offset        = serialToHost16(payload + length);
    const uint8_t slotNr         = payload[length + sizeof(offset)];

    return processingRadar->writeCustomWindowCoefficients(processingRadar, slotNr, offset, count, coefficients);
}

uint8_t Commands_IProcessingRadar_reinit(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return processingRadar->reinitialize(processingRadar);
}

uint8_t Commands_IProcessingRadar_start(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != 0)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    return processingRadar->start(processingRadar);
}

uint8_t Commands_IProcessingRadar_isBusy(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(uint8_t))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    bool state    = processingRadar->isBusy(processingRadar);
    (*payload)[0] = (uint8_t)state;
    return STATUS_SUCCESS;
}

static inline uint8_t checkCommandTarget(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface)
{
    if (bId >= m_processingCount)
    {
        return STATUS_COMMAND_ID_INVALID;
    }
    if (m_processing[bId] == NULL)
    {
        return STATUS_COMMAND_ID_INVALID;
    }

    if (bImplementation != COMPONENT_IMPL_DEFAULT)
    {
        //No special implementations available
        return STATUS_COMMAND_IMPL_INVALID;
    }

    if (bSubinterface != COMPONENT_SUBIF_DEFAULT)
    {
        //No subinterface implementations available
        return STATUS_COMMAND_SUBIF_INVALID;
    }

    return STATUS_SUCCESS;
}

static inline uint8_t Commands_IProcessingRadar_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IProcessingRadar *processingRadar = m_processing[bId];

    switch (bFunction)
    {
        case FN_GET_IMPL:
            return Commands_IProcessingRadar_getImplementation(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_IS_BUSY:
            return Commands_IProcessingRadar_isBusy(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_READ_OPERATION_RESULT:
            return Commands_IProcessingRadar_readOperationResult(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_GET_RAW_DATA:
            return Commands_IProcessingRadar_getRawData(processingRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IProcessingRadar_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IProcessingRadar *processingRadar = m_processing[bId];

    switch (bFunction)
    {
        case FN_PROCESSING_RADAR_WRITE_CONFIG_RAM:
            return Commands_IProcessingRadar_writeConfigRam(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_WRITE_CUSTOM_WINDOW_COEFFICIENTS:
            return Commands_IProcessingRadar_writeCustomWindowCoefficients(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_CONFIGURE:
            return Commands_IProcessingRadar_configure(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_DO_FFT:
            return Commands_IProcessingRadar_doFft(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_DO_NCI:
            return Commands_IProcessingRadar_doNci(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_DO_PSD:
            return Commands_IProcessingRadar_doPsd(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_DO_THRESHOLDING:
            return Commands_IProcessingRadar_doThresholding(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_REINIT:
            return Commands_IProcessingRadar_reinit(processingRadar, wLength, payload);
            break;
        case FN_PROCESSING_RADAR_START:
            return Commands_IProcessingRadar_start(processingRadar, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

static inline uint8_t Commands_IProcessingRadar_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    //IProcessingRadar *processingRadar = m_processing[bId];

    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

void Commands_IProcessingRadar_Constructor(ICommands *self)
{
    m_processingCount = 0;
    self->m_bType     = COMPONENT_TYPE_PROCESSING_RADAR;
    self->read        = Commands_IProcessingRadar_read;
    self->write       = Commands_IProcessingRadar_write;
    self->transfer    = Commands_IProcessingRadar_transfer;
}

bool Commands_IProcessingRadar_registerIProcessingRadar(IProcessingRadar *processing)
{
    if (m_processingCount >= MAX_INSTANCE_REGISTRATIONS)
    {
        return false;
    }
    m_processing[m_processingCount] = processing;
    m_processingCount++;
    return true;
}

/*  @} */
