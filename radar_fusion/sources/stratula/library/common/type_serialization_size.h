/****************************************************************************\
* Copyright (C) 2017 Infineon Technologies
*
* THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
\****************************************************************************/
#ifndef TYPE_SERIALIZATION_SIZE_H
#define TYPE_SERIALIZATION_SIZE_H 1

#include <common/typeutils.h>
#include <stddef.h>

#include <components/interfaces/IRadarAvian.h>
#include <platform/interfaces/IProcessingRadar.h>
#include <universal/data_definitions.h>
#include <universal/types/IfxRfe_Types.h>
#include <universal/types/IfxRsp_Types.h>


static inline size_t sizeof_serialized_IfxRfe_MmicConfig(void)
{
    return sizeof(((IfxRfe_MmicConfig *)0)->enableMonitoring) +
           sizeof(((IfxRfe_MmicConfig *)0)->sampleWidth) +
           sizeof(((IfxRfe_MmicConfig *)0)->sampleRate) +
           sizeof(((IfxRfe_MmicConfig *)0)->txPower) +
           sizeof(((IfxRfe_MmicConfig *)0)->lpGain) +
           sizeof(((IfxRfe_MmicConfig *)0)->mixerGain) +
           sizeof(((IfxRfe_MmicConfig *)0)->dcocEnable) +
           sizeof(((IfxRfe_MmicConfig *)0)->dcocShift);
}

static inline size_t sizeof_serialized_IfxRfe_Ramp(void)
{
    return sizeof(((IfxRfe_Ramp *)0)->fStart) +
           sizeof(((IfxRfe_Ramp *)0)->fDelta) +
           sizeof(((IfxRfe_Ramp *)0)->loops) +
           sizeof(((IfxRfe_Ramp *)0)->muxOut) +
           sizeof(((IfxRfe_Ramp *)0)->txMask) +
           sizeof(((IfxRfe_Ramp *)0)->txPhases);
}

static inline size_t sizeof_serialized_IfxRfe_Sequence(uint8_t rampCount)
{
    return rampCount * sizeof_serialized_IfxRfe_Ramp() +
           sizeof(((IfxRfe_Sequence *)0)->tRamp) +
           sizeof(((IfxRfe_Sequence *)0)->tJump) +
           sizeof(((IfxRfe_Sequence *)0)->tWait) +
           sizeof(((IfxRfe_Sequence *)0)->tRampStartDelay) +
           sizeof(((IfxRfe_Sequence *)0)->tRampStopDelay) +
           sizeof(((IfxRfe_Sequence *)0)->rxMask) +
           sizeof(((IfxRfe_Sequence *)0)->loops) +
           sizeof(((IfxRfe_Sequence *)0)->rampCount);
}

static inline size_t sizeof_serialized_IDataProperties(void)
{
    return sizeof(((IDataProperties_t *)0)->format) +
           sizeof(((IDataProperties_t *)0)->rxChannels) +
           sizeof(((IDataProperties_t *)0)->samples) +
           sizeof(((IDataProperties_t *)0)->ramps) +
           sizeof(((IDataProperties_t *)0)->channelSwapping) +
           sizeof(((IDataProperties_t *)0)->bitWidth);
}

static inline size_t sizeof_serialized_IProcessingRadarInput(void)
{
    return sizeof(((IProcessingRadarInput_t *)0)->txChannels) +
           sizeof(((IProcessingRadarInput_t *)0)->virtualAnt) +
           sizeof(((IProcessingRadarInput_t *)0)->rampsPerTx) +
           sizeof(((IProcessingRadarInput_t *)0)->maxRange) +
           sizeof(((IProcessingRadarInput_t *)0)->maxVelocity);
}

static inline size_t sizeof_serialized_IfxRsp_FftSetting(void)
{
    return sizeof(((IfxRsp_FftSetting *)0)->size) +          ///< 0 = default number of samples (= smallest power of 2 greater than or equal to number of samples)
           sizeof(((IfxRsp_FftSetting *)0)->acceptedBins) +  ///< 0 = all (disable rejection), otherwise number of accepted bins counting after rejected bins
           sizeof(((IfxRsp_FftSetting *)0)->window) +
           sizeof(((IfxRsp_FftSetting *)0)->windowFormat) +  ///< only Q15 (default) or Q31
           sizeof(((IfxRsp_FftSetting *)0)->exponent) +      ///< scale result (only for 16bit output data format)
           sizeof(((IfxRsp_FftSetting *)0)->flags);
}

static inline size_t sizeof_serialized_IfxRsp_LocalMaxSetting(void)
{
    return sizeof(((IfxRsp_LocalMaxSetting *)0)->mode) +
           sizeof(((IfxRsp_LocalMaxSetting *)0)->threshold) +
           sizeof(((IfxRsp_LocalMaxSetting *)0)->windowWidth) +
           sizeof(((IfxRsp_LocalMaxSetting *)0)->combineAnd);
}

static inline size_t sizeof_serialized_IfxRsp_CfarCaSetting(void)
{
    return sizeof(((IfxRsp_CfarCaSetting *)0)->algorithm) +
           sizeof(((IfxRsp_CfarCaSetting *)0)->guardCells) +
           sizeof(((IfxRsp_CfarCaSetting *)0)->windowCellsExponent) +
           sizeof(((IfxRsp_CfarCaSetting *)0)->cashSubWindowExponent) +
           sizeof(((IfxRsp_CfarCaSetting *)0)->betaThreshold);
}

static inline size_t sizeof_serialized_IfxRsp_CfarGosSetting(void)
{
    return sizeof(((IfxRsp_CfarGosSetting *)0)->algorithm) +
           sizeof(((IfxRsp_CfarGosSetting *)0)->guardCells) +
           sizeof(((IfxRsp_CfarGosSetting *)0)->indexLead) +
           sizeof(((IfxRsp_CfarGosSetting *)0)->indexLag) +
           sizeof(((IfxRsp_CfarGosSetting *)0)->windowCells) +
           sizeof(((IfxRsp_CfarGosSetting *)0)->betaThreshold);
}

static inline size_t sizeof_serialized_IfxRsp_ThresholdingSetting(void)
{
    return sizeof(((IfxRsp_ThresholdingSetting *)0)->spectrumExtension) +
           sizeof_serialized_IfxRsp_LocalMaxSetting() +
           sizeof_serialized_IfxRsp_CfarCaSetting() +
           sizeof_serialized_IfxRsp_CfarGosSetting();
}

static inline size_t sizeof_serialized_IfxRsp_DetectionSettings(void)
{
    return sizeof(((IfxRsp_DetectionSettings *)0)->maxDetections) +
           sizeof(((IfxRsp_DetectionSettings *)0)->fftSize) +
           sizeof(((IfxRsp_DetectionSettings *)0)->flags) +
           sizeof_serialized_IfxRsp_ThresholdingSetting() * ARRAY_SIZE(((IfxRsp_DetectionSettings *)0)->thresholdingSettings);
}

static inline size_t sizeof_serialized_IfxRsp_DbfSetting(void)
{
    return sizeof(((IfxRsp_DbfSetting *)0)->angles) +             ///< number of angles to calculate
           sizeof(((IfxRsp_DbfSetting *)0)->coefficientFormat) +  ///< only Complex16 or Complex32 (default)
           sizeof(((IfxRsp_DbfSetting *)0)->format) +             ///< only Complex16 or Complex32 (default)
           sizeof(((IfxRsp_DbfSetting *)0)->centerAngle) +        ///< [rad]
           sizeof(((IfxRsp_DbfSetting *)0)->angularSpacing) +     ///< [rad]
           sizeof_serialized_IfxRsp_ThresholdingSetting();
}

static inline size_t sizeof_serialized_IfxRsp_AntennaCalibration(void)
{
    return sizeof(((IfxRsp_AntennaCalibration *)0)->spacing) +  ///< antenna spacing in multiples of lambda
           sizeof(((IfxRsp_AntennaCalibration *)0)->temperature) +
           sizeof(((IfxRsp_AntennaCalibration *)0)->fftSize) +      ///< fftSize used (0 = no Calibration)
           sizeof(((IfxRsp_AntennaCalibration *)0)->count) +        ///< number of virtual antennae
           sizeof(((IfxRsp_AntennaCalibration *)0)->indices) +      ///< virtual antenna ordering
           sizeof(((IfxRsp_AntennaCalibration *)0)->coefficients);  ///< calibration coefficients as complex values (float[2])
}

static inline size_t sizeof_serialized_IfxRsp_Stages(void)
{
    return sizeof(((IfxRsp_Stages *)0)->fftSteps) +   ///< number of FFT stages enabled (0: time data; 1: Range FFT; 2: Doppler)
           sizeof(((IfxRsp_Stages *)0)->fftFormat) +  ///< output data format for FFT, only Real* or Complex*
           sizeof(((IfxRsp_Stages *)0)->nciFormat) +
           sizeof(((IfxRsp_Stages *)0)->virtualChannels) +
           sizeof_serialized_IfxRsp_FftSetting() * ARRAY_SIZE(((IfxRsp_Stages *)0)->fftSettings) +
           sizeof_serialized_IfxRsp_DetectionSettings() +
           sizeof_serialized_IfxRsp_DbfSetting() * ARRAY_SIZE(((IfxRsp_Stages *)0)->dbfSetting);
}

static inline size_t sizeof_serialized_IfxRsp_Signal(void)
{
    return sizeof(((IfxRsp_Signal *)0)->size) +
           sizeof(((IfxRsp_Signal *)0)->baseAddress) +
           sizeof(((IfxRsp_Signal *)0)->stride) +
           sizeof(((IfxRsp_Signal *)0)->rows) +
           sizeof(((IfxRsp_Signal *)0)->cols) +
           sizeof(((IfxRsp_Signal *)0)->pages) +
           sizeof(((IfxRsp_Signal *)0)->format);
}

#endif /* TYPE_SERIALIZATION_SIZE_H */
