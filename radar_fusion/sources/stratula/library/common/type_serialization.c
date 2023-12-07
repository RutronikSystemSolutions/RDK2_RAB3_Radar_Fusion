#include "type_serialization.h"

#include <common/binutils.h>
#include <common/serialization.h>


uint8_t *hostToSerial_IfxRfe_MmicConfig(uint8_t buf[], const IfxRfe_MmicConfig *c)
{
    buf = hostToSerial(buf, c->enableMonitoring);
    buf = hostToSerial(buf, c->sampleWidth);
    buf = hostToSerial(buf, c->sampleRate);
    buf = hostToSerial(buf, c->txPower);
    buf = hostToSerial(buf, c->lpGain);
    buf = hostToSerial(buf, c->mixerGain);
    buf = hostToSerial(buf, c->dcocEnable);
    buf = hostToSerial(buf, c->dcocShift);

    return buf;
}

const uint8_t *serialToHost_IfxRfe_MmicConfig(const uint8_t buf[], IfxRfe_MmicConfig *c)
{
    buf = serialToHost(buf, c->enableMonitoring);
    buf = serialToHost(buf, c->sampleWidth);
    buf = serialToHost(buf, c->sampleRate);
    buf = serialToHost(buf, c->txPower);
    buf = serialToHost(buf, c->lpGain);
    buf = serialToHost(buf, c->mixerGain);
    buf = serialToHost(buf, c->dcocEnable);
    buf = serialToHost(buf, c->dcocShift);

    return buf;
}

uint8_t *hostToSerial_IfxRfe_Ramps(uint8_t buf[], const IfxRfe_Ramp r[], const uint8_t rampCount)
{
    for (unsigned int i = 0; i < rampCount; i++)
    {
        buf = hostToSerial(buf, r[i].fStart);
        buf = hostToSerial(buf, r[i].fDelta);
        buf = hostToSerial(buf, r[i].loops);
        buf = hostToSerial(buf, r[i].muxOut);
        buf = hostToSerial(buf, r[i].txMask);
        for (unsigned int j = 0; j < ARRAY_SIZE(r[i].txPhases); j++)
        {
            buf = hostToSerial(buf, r[i].txPhases[j]);
        }
    }

    return buf;
}

uint8_t *hostToSerial_IfxRfe_Sequence(uint8_t buf[], const IfxRfe_Sequence *s)
{
    buf = hostToSerial(buf, s->tRamp);
    buf = hostToSerial(buf, s->tJump);
    buf = hostToSerial(buf, s->tWait);
    buf = hostToSerial(buf, s->tRampStartDelay);
    buf = hostToSerial(buf, s->tRampStopDelay);
    buf = hostToSerial(buf, s->rxMask);
    buf = hostToSerial(buf, s->loops);
    buf = hostToSerial(buf, s->rampCount);

    return buf;
}

const uint8_t *serialToHost_IfxRfe_Ramps(const uint8_t buf[], IfxRfe_Ramp r[], const uint8_t rampCount)
{
    for (unsigned int i = 0; i < rampCount; i++)
    {
        buf = serialToHost(buf, r[i].fStart);
        buf = serialToHost(buf, r[i].fDelta);
        buf = serialToHost(buf, r[i].loops);
        buf = serialToHost(buf, r[i].muxOut);
        buf = serialToHost(buf, r[i].txMask);
        for (unsigned int j = 0; j < ARRAY_SIZE(r[i].txPhases); j++)
        {
            buf = serialToHost(buf, r[i].txPhases[j]);
        }
    }

    return buf;
}

const uint8_t *serialToHost_IfxRfe_Sequence(const uint8_t buf[], IfxRfe_Sequence *s)
{
    buf = serialToHost(buf, s->tRamp);
    buf = serialToHost(buf, s->tJump);
    buf = serialToHost(buf, s->tWait);
    buf = serialToHost(buf, s->tRampStartDelay);
    buf = serialToHost(buf, s->tRampStopDelay);
    buf = serialToHost(buf, s->rxMask);
    buf = serialToHost(buf, s->loops);
    buf = serialToHost(buf, s->rampCount);

    return buf;
}

uint8_t *hostToSerial_IDataProperties(uint8_t buf[], const IDataProperties_t *p)
{
    buf = hostToSerial(buf, p->format);
    buf = hostToSerial(buf, p->rxChannels);
    buf = hostToSerial(buf, p->ramps);
    buf = hostToSerial(buf, p->samples);
    buf = hostToSerial(buf, p->channelSwapping);
    buf = hostToSerial(buf, p->bitWidth);

    return buf;
}

const uint8_t *serialToHost_IDataProperties(const uint8_t buf[], IDataProperties_t *p)
{
    buf = serialToHost(buf, p->format);
    buf = serialToHost(buf, p->rxChannels);
    buf = serialToHost(buf, p->ramps);
    buf = serialToHost(buf, p->samples);
    buf = serialToHost(buf, p->channelSwapping);
    buf = serialToHost(buf, p->bitWidth);

    return buf;
}

uint8_t *hostToSerial_IProcessingRadarInput(uint8_t buf[], const IProcessingRadarInput_t *c)
{
    buf = hostToSerial(buf, c->txChannels);
    buf = hostToSerial(buf, c->virtualAnt);
    buf = hostToSerial(buf, c->rampsPerTx);
    buf = hostToSerial(buf, c->maxRange);
    buf = hostToSerial(buf, c->maxVelocity);

    return buf;
}

const uint8_t *serialToHost_IProcessingRadarInput(const uint8_t buf[], IProcessingRadarInput_t *c)
{
    buf = serialToHost(buf, c->txChannels);
    buf = serialToHost(buf, c->virtualAnt);
    buf = serialToHost(buf, c->rampsPerTx);
    buf = serialToHost(buf, c->maxRange);
    buf = serialToHost(buf, c->maxVelocity);

    return buf;
}

uint8_t *hostToSerial_IfxRsp_FftSetting(uint8_t buf[], const IfxRsp_FftSetting *c)
{
    buf = hostToSerial(buf, c->size);          ///< 0 = default number of samples (= smallest power of 2 greater than or equal to number of samples)
    buf = hostToSerial(buf, c->acceptedBins);  ///< 0 = all (disable rejection), otherwise number of accepted bins counting from first bin
    buf = hostToSerial(buf, c->window);
    buf = hostToSerial(buf, c->windowFormat);  ///< only Q15 (default) or Q31
    buf = hostToSerial(buf, c->exponent);      ///< scale result (only for 16bit output data format)
    buf = hostToSerial(buf, c->flags);

    return buf;
}

const uint8_t *serialToHost_IfxRsp_FftSetting(const uint8_t buf[], IfxRsp_FftSetting *c)
{
    buf = serialToHost(buf, c->size);          ///< 0 = default number of samples (= smallest power of 2 greater than or equal to number of samples)
    buf = serialToHost(buf, c->acceptedBins);  ///< 0 = all (disable rejection), otherwise number of accepted bins counting from first bin
    buf = serialToHost(buf, c->window);
    buf = serialToHost(buf, c->windowFormat);  ///< only Q15 (default) or Q31
    buf = serialToHost(buf, c->exponent);      ///< scale result (only for 16bit output data format)
    buf = serialToHost(buf, c->flags);

    return buf;
}

uint8_t *hostToSerial_IfxRsp_LocalMaxSetting(uint8_t buf[], const IfxRsp_LocalMaxSetting *c)
{
    buf = hostToSerial(buf, c->mode);
    buf = hostToSerial(buf, c->threshold);    ///< 0..0xFFFFFFFF
    buf = hostToSerial(buf, c->windowWidth);  ///< 0,1,2
    buf = hostToSerial(buf, c->combineAnd);

    return buf;
}

const uint8_t *serialToHost_IfxRsp_LocalMaxSetting(const uint8_t buf[], IfxRsp_LocalMaxSetting *c)
{
    buf = serialToHost(buf, c->mode);
    buf = serialToHost(buf, c->threshold);    ///< 0..0xFFFFFFFF
    buf = serialToHost(buf, c->windowWidth);  ///< 0,1,2
    buf = serialToHost(buf, c->combineAnd);

    return buf;
}

uint8_t *hostToSerial_IfxRsp_CfarCaSetting(uint8_t buf[], const IfxRsp_CfarCaSetting *c)
{
    buf = hostToSerial(buf, c->algorithm);
    buf = hostToSerial(buf, c->guardCells);             ///< 0..32
    buf = hostToSerial(buf, c->windowCellsExponent);    ///< 0..5
    buf = hostToSerial(buf, c->cashSubWindowExponent);  ///< 0..5
    buf = hostToSerial(buf, c->betaThreshold);          ///< 0..0xFFFF

    return buf;
}

const uint8_t *serialToHost_IfxRsp_CfarCaSetting(const uint8_t buf[], IfxRsp_CfarCaSetting *c)
{
    buf = serialToHost(buf, c->algorithm);
    buf = serialToHost(buf, c->guardCells);             ///< 0..32
    buf = serialToHost(buf, c->windowCellsExponent);    ///< 0..5
    buf = serialToHost(buf, c->cashSubWindowExponent);  ///< 0..5
    buf = serialToHost(buf, c->betaThreshold);          ///< 0..0xFFFF

    return buf;
}

uint8_t *hostToSerial_IfxRsp_CfarGosSetting(uint8_t buf[], const IfxRsp_CfarGosSetting *c)
{
    buf = hostToSerial(buf, c->algorithm);
    buf = hostToSerial(buf, c->guardCells);     ///< 0..32
    buf = hostToSerial(buf, c->indexLead);      ///< 1..32
    buf = hostToSerial(buf, c->indexLag);       ///< 1..32
    buf = hostToSerial(buf, c->windowCells);    ///< 1..32
    buf = hostToSerial(buf, c->betaThreshold);  ///< 0..0xFFFF

    return buf;
}

const uint8_t *serialToHost_IfxRsp_CfarGosSetting(const uint8_t buf[], IfxRsp_CfarGosSetting *c)
{
    buf = serialToHost(buf, c->algorithm);
    buf = serialToHost(buf, c->guardCells);     ///< 0..32
    buf = serialToHost(buf, c->indexLead);      ///< 1..32
    buf = serialToHost(buf, c->indexLag);       ///< 1..32
    buf = serialToHost(buf, c->windowCells);    ///< 1..32
    buf = serialToHost(buf, c->betaThreshold);  ///< 0..0xFFFF

    return buf;
}

uint8_t *hostToSerial_IfxRsp_ThresholdingSetting(uint8_t buf[], const IfxRsp_ThresholdingSetting *c)
{
    buf = hostToSerial(buf, c->spectrumExtension);
    buf = hostToSerial_IfxRsp_LocalMaxSetting(buf, &c->localMax);
    buf = hostToSerial_IfxRsp_CfarCaSetting(buf, &c->cfarCa);
    buf = hostToSerial_IfxRsp_CfarGosSetting(buf, &c->cfarGos);

    return buf;
}

const uint8_t *serialToHost_IfxRsp_ThresholdingSetting(const uint8_t buf[], IfxRsp_ThresholdingSetting *c)
{
    buf = serialToHost(buf, c->spectrumExtension);
    buf = serialToHost_IfxRsp_LocalMaxSetting(buf, &c->localMax);
    buf = serialToHost_IfxRsp_CfarCaSetting(buf, &c->cfarCa);
    buf = serialToHost_IfxRsp_CfarGosSetting(buf, &c->cfarGos);

    return buf;
}

uint8_t *hostToSerial_IfxRsp_DetectionSettings(uint8_t buf[], const IfxRsp_DetectionSettings *c)
{
    buf = hostToSerial(buf, c->maxDetections);
    buf = hostToSerial(buf, c->fftSize);
    buf = hostToSerial(buf, c->flags);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->thresholdingSettings); i++)
    {
        buf = hostToSerial_IfxRsp_ThresholdingSetting(buf, &c->thresholdingSettings[i]);
    }
    return buf;
}

const uint8_t *serialToHost_IfxRsp_DetectionSettings(const uint8_t buf[], IfxRsp_DetectionSettings *c)
{
    buf = serialToHost(buf, c->maxDetections);
    buf = serialToHost(buf, c->fftSize);
    buf = serialToHost(buf, c->flags);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->thresholdingSettings); i++)
    {
        buf = serialToHost_IfxRsp_ThresholdingSetting(buf, &c->thresholdingSettings[i]);
    }

    return buf;
}

uint8_t *hostToSerial_IfxRsp_DbfSetting(uint8_t buf[], const IfxRsp_DbfSetting *c)
{
    buf = hostToSerial(buf, c->angles);             ///< number of angles to calculate
    buf = hostToSerial(buf, c->coefficientFormat);  ///< only Complex16 or Complex32 (default)
    buf = hostToSerial(buf, c->format);             ///< only Complex16 or Complex32 (default)
    buf = hostToSerial(buf, c->centerAngle);        ///< [rad]
    buf = hostToSerial(buf, c->angularSpacing);     ///< [rad]
    buf = hostToSerial_IfxRsp_ThresholdingSetting(buf, &c->thresholding);

    return buf;
}

const uint8_t *serialToHost_IfxRsp_DbfSetting(const uint8_t buf[], IfxRsp_DbfSetting *c)
{
    buf = serialToHost(buf, c->angles);             ///< number of angles to calculate
    buf = serialToHost(buf, c->coefficientFormat);  ///< only Complex16 or Complex32 (default)
    buf = serialToHost(buf, c->format);             ///< only Complex16 or Complex32 (default)
    buf = serialToHost(buf, c->centerAngle);        ///< [rad]
    buf = serialToHost(buf, c->angularSpacing);     ///< [rad]
    buf = serialToHost_IfxRsp_ThresholdingSetting(buf, &c->thresholding);

    return buf;
}

uint8_t *hostToSerial_IfxRsp_AntennaCalibration(uint8_t buf[], const IfxRsp_AntennaCalibration *c)
{
    buf = hostToSerial(buf, c->spacing);  ///< antenna spacing in multiples of lambda
    buf = hostToSerial(buf, c->temperature);
    buf = hostToSerial(buf, c->fftSize);       ///< fftSize used (0 = no Calibration)
    buf = hostToSerial(buf, c->count);         ///< number of virtual antennae
    buf = hostToSerial(buf, c->indices);       ///< virtual antenna ordering
    buf = hostToSerial(buf, c->coefficients);  ///< calibration coefficients as complex values (float[2])

    return buf;
}

const uint8_t *serialToHost_IfxRsp_AntennaCalibration(const uint8_t buf[], IfxRsp_AntennaCalibration *c)
{
    buf = serialToHost(buf, c->spacing);  ///< antenna spacing in multiples of lambda
    buf = serialToHost(buf, c->temperature);
    buf = serialToHost(buf, c->fftSize);       ///< fftSize used (0 = no Calibration)
    buf = serialToHost(buf, c->count);         ///< number of virtual antennae
    buf = serialToHost(buf, c->indices);       ///< virtual antenna ordering
    buf = serialToHost(buf, c->coefficients);  ///< calibration coefficients as complex values (float[2])

    return buf;
}

uint8_t *hostToSerial_IfxRsp_Stages(uint8_t buf[], const IfxRsp_Stages *c)
{
    buf = hostToSerial(buf, c->fftSteps);   ///< output data format for FFT, only Real* or Complex*
    buf = hostToSerial(buf, c->fftFormat);  ///< number of FFT stages enabled (0: time data; 1: Range FFT; 2: Doppler)
    buf = hostToSerial(buf, c->nciFormat);
    buf = hostToSerial(buf, c->virtualChannels);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->fftSettings); i++)
    {
        buf = hostToSerial_IfxRsp_FftSetting(buf, &c->fftSettings[i]);
    }
    buf = hostToSerial_IfxRsp_DetectionSettings(buf, &c->detectionSettings);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->dbfSetting); i++)
    {
        buf = hostToSerial_IfxRsp_DbfSetting(buf, &c->dbfSetting[i]);
    }

    return buf;
}

const uint8_t *serialToHost_IfxRsp_Stages(const uint8_t buf[], IfxRsp_Stages *c)
{
    buf = serialToHost(buf, c->fftSteps);   ///< output data format for FFT, only Real* or Complex*
    buf = serialToHost(buf, c->fftFormat);  ///< number of FFT stages enabled (0: time data; 1: Range FFT; 2: Doppler)
    buf = serialToHost(buf, c->nciFormat);
    buf = serialToHost(buf, c->virtualChannels);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->fftSettings); i++)
    {
        buf = serialToHost_IfxRsp_FftSetting(buf, &c->fftSettings[i]);
    }
    buf = serialToHost_IfxRsp_DetectionSettings(buf, &c->detectionSettings);
    for (unsigned int i = 0; i < ARRAY_SIZE(c->dbfSetting); i++)
    {
        buf = serialToHost_IfxRsp_DbfSetting(buf, &c->dbfSetting[i]);
    }

    return buf;
}

uint8_t *hostToSerial_IfxRsp_Signal(uint8_t buf[], const IfxRsp_Signal *s)
{
    buf = hostToSerial(buf, s->size);
    buf = hostToSerial(buf, s->baseAddress);
    buf = hostToSerial(buf, s->stride);
    buf = hostToSerial(buf, s->rows);
    buf = hostToSerial(buf, s->cols);
    buf = hostToSerial(buf, s->pages);
    buf = hostToSerial(buf, s->format);

    return buf;
}

const uint8_t *serialToHost_IfxRsp_Signal(const uint8_t buf[], IfxRsp_Signal *s)
{
    buf = serialToHost(buf, s->size);
    buf = serialToHost(buf, s->baseAddress);
    buf = serialToHost(buf, s->stride);
    buf = serialToHost(buf, s->rows);
    buf = serialToHost(buf, s->cols);
    buf = serialToHost(buf, s->pages);
    buf = serialToHost(buf, s->format);

    return buf;
}
