/**
 * \addtogroup      IProcessingRadar
 * \brief
 * This an interface supporting radar-data signal processing,
 * as specified by IProcessingRadar_Descriptor
 * @{
 */
#ifndef IPROCESSING_RADAR_H
#define IPROCESSING_RADAR_H 1

#include <common/errors.h>
#include <universal/data_definitions.h>
#include <universal/types/IfxRsp_Types.h>


typedef struct _IProcessingRadarInput_t
{
    uint8_t txChannels;   ///< no. of transmit channels
    uint8_t virtualAnt;   ///< no. of virtual antennae
    uint16_t rampsPerTx;  ///< no. of ramps per transmit channel

    float maxRange;
    float maxVelocity;

} IProcessingRadarInput_t;


typedef struct _IProcessingRadar IProcessingRadar;

struct _IProcessingRadar
{
    sr_t (*getRawData)(IProcessingRadar *self, IfxRsp_Signal **data);
    IfxRsp_Targets *(*getTargetProperties)(IProcessingRadar *self);
    sr_t (*configure)(IProcessingRadar *self, uint8_t dataSource, const IDataProperties_t *dataProperties, const IProcessingRadarInput_t *radarInfo,
                      const IfxRsp_Stages *stages, const IfxRsp_AntennaCalibration *antennaConfig);

    sr_t (*doFft)(IProcessingRadar *self, const IfxRsp_Signal *input, const IfxRsp_FftSetting *settings, IfxRsp_Signal *output, uint16_t samples, uint16_t offset, uint8_t dimension, uint8_t format);
    sr_t (*doNci)(IProcessingRadar *self, const IfxRsp_Signal *input, uint8_t format, IfxRsp_Signal *output);
    sr_t (*doThresholding)(IProcessingRadar *self, const IfxRsp_Signal *input, uint8_t dimension, const IfxRsp_ThresholdingSetting *settings, IfxRsp_Signal *output);
    sr_t (*doPsd)(IProcessingRadar *self, const IfxRsp_Signal *input, uint16_t nFft, IfxRsp_Signal *output);

    sr_t (*writeCustomWindowCoefficients)(IProcessingRadar *self, uint8_t slotNr, uint16_t offset, uint16_t count, const uint32_t *coefficients);

    /** Write configuration directly into processing-unit's Ram
     *
     * @param offset Offset in the configuration RAM (in 32-bit words)
     * @param count  number of 32-bit words to write into the configuration RAM
     * @param *ramContent 32-bit configuration values
     * @return Strata error code
     *
     */
    sr_t (*writeConfigRam)(IProcessingRadar *self, uint16_t offset, uint16_t count, const uint32_t *ramContent);

    /** Reset and re-initialize the processing-unit
     * @return Strata error code
     */
    sr_t (*reinitialize)(IProcessingRadar *self);

    /** Start processing as configured. After started, it will be in a busy state
     * until all configured data has been received and processed.
     *
     * @return Strata error code
     */
    sr_t (*start)(IProcessingRadar *self);

    /** @return true if it is still busy after started, false otherwise
     */
    bool (*isBusy)(IProcessingRadar *self);

    uint32_t (*allocateCommonMemory)(IProcessingRadar *self, uint32_t size);

    /**
     * Register a callback for processing the data-measurement
     */
    void (*registerConsumerCallback)(IProcessingRadar *self, IProcessing_DataCallback dataCallback, void *arg);

    /**
     * Special implementation of the IProcessingRadar interface
     * Must be set by constructor of the derived "class"
     */
    uint8_t m_bImplementation;
};

#endif /* IPROCESSING_RADAR_H */

/** @} */
