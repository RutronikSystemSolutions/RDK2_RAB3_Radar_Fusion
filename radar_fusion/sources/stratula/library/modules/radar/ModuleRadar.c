#include "ModuleRadar.h"
#include <common/binutils.h>
#include <string.h>


#define M_C0 299792458.0  //Speed of light in vacuum (m/s)

#define IfxRsp_Modulation_Tdm     0
#define IfxRsp_Modulation_Cdm     1
#define IfxRsp_Modulation_Unknown 2


#define SELF ((ModuleRadar *)(uintptr_t)self)


static void ModuleRadar_registerDataCallback(IModule *self, IProcessing_DataCallback dataCallback, void *arg)
{
    Module_registerDataCallback(&SELF->base, dataCallback, arg);
}

static uint8_t getModulation(const IfxRfe_Sequence *s)
{
    if (s->rampCount == 0)
    {
        return IfxRsp_Modulation_Tdm;
    }

    const unsigned int count = getBitCount(s->ramps[0].txMask);
    uint_fast8_t i;
    for (i = 1; i < s->rampCount; i++)
    {
        if (count != getBitCount(s->ramps[i].txMask))
        {
            break;
        }
    }
    if (i == s->rampCount)  // for loop did not break
    {
        if (count == 1)
        {
            return IfxRsp_Modulation_Tdm;
        }
        else
        {
            return IfxRsp_Modulation_Cdm;
        }
    }

    return IfxRsp_Modulation_Unknown;
}

static sr_t ModuleRadar_getDataProperties(IModuleRadar *self, IDataProperties_t *props)
{
    const IfxRfe_MmicConfig *c = SELF->m_mmicConfig;
    const IfxRfe_Sequence *s   = SELF->m_sequence;

    props->format = DataFormat_Q15;

    props->bitWidth = c->sampleWidth;
    props->samples  = (uint16_t)(0.5 + (s->tRamp - s->tRampStartDelay + s->tRampStopDelay) * c->sampleRate);

    uint16_t ramps = 0;
    for (uint_fast8_t i = 0; i < s->rampCount; i++)
    {
        ramps += s->ramps[i].loops;
    }
    props->ramps = s->loops * ramps;

    props->rxChannels = getBitCount(s->rxMask);
    return E_SUCCESS;
}

static sr_t ModuleRadar_getRadarInfo(IModuleRadar *self, IProcessingRadarInput_t *info, const IDataProperties_t *dataProperties)
{
    const IfxRfe_MmicConfig *c = SELF->m_mmicConfig;
    const IfxRfe_Sequence *s   = SELF->m_sequence;

    uint8_t rxChannels;
    uint16_t ramps;
    if (dataProperties)
    {
        rxChannels = dataProperties->rxChannels;
        ramps      = dataProperties->ramps;
    }
    else
    {
        IDataProperties_t dp;
        RETURN_ON_ERROR(self->getDataProperties(self, &dp));
        rxChannels = dp.rxChannels;
        ramps      = dp.ramps;
    }

    uint16_t combinedTxMask = 0x0000;
    for (uint_fast8_t i = 0; i < s->rampCount; i++)
    {
        combinedTxMask |= s->ramps[i].txMask;
    }
    info->txChannels = getBitCount(combinedTxMask);

    double tRepetition = s->tRamp + s->tWait + s->tJump;

    uint8_t modulation = getModulation(s);
    if (modulation == IfxRsp_Modulation_Tdm)
    {
        tRepetition *= info->txChannels;
        info->virtualAnt = rxChannels * info->txChannels;
        info->rampsPerTx = ramps / info->txChannels;
    }
    else
    {
        info->virtualAnt = rxChannels;
        info->rampsPerTx = ramps;
    }

    double tRamp  = s->tRamp;
    double fDelta = s->ramps[0].fDelta;
    if (fDelta < 0)
    {
        fDelta = -fDelta;
    }
    info->maxRange    = M_C0 / 4 * c->sampleRate * tRamp / fDelta;
    double fCenter    = s->ramps[0].fStart + s->ramps[0].fDelta / 2;
    info->maxVelocity = M_C0 / (4 * fCenter * tRepetition);
    return E_SUCCESS;
}

static IfxRfe_MmicConfig *ModuleRadar_getConfiguration(IModuleRadar *self)
{
    return SELF->m_mmicConfig;
}

static IfxRfe_Sequence *ModuleRadar_getSequence(IModuleRadar *self)
{
    return SELF->m_sequence;
}

static IfxRsp_Stages *ModuleRadar_getProcessingStages(IModuleRadar *self)
{
    return SELF->m_processingStages;
}

static IfxRsp_AntennaCalibration *ModuleRadar_getCalibration(IModuleRadar *self)
{
    return SELF->m_calibration;
}

sr_t ModuleRadar_setConfiguration(IModuleRadar *self, const IfxRfe_MmicConfig *c)
{
    *SELF->m_mmicConfig = *c;
    return E_SUCCESS;
}

sr_t ModuleRadar_setSequence(IModuleRadar *self, const IfxRfe_Sequence *s)
{
    IfxRfe_Ramp *ramps = SELF->m_sequence->ramps;
    *SELF->m_sequence  = *s;
    memcpy(ramps, s->ramps, s->rampCount * sizeof(IfxRfe_Ramp));
    SELF->m_sequence->ramps = ramps;
    return E_SUCCESS;
}

sr_t ModuleRadar_setProcessingStages(IModuleRadar *self, const IfxRsp_Stages *s)
{
    *SELF->m_processingStages = *s;
    return E_SUCCESS;
}

sr_t ModuleRadar_setCalibration(IModuleRadar *self, const IfxRsp_AntennaCalibration c[2])
{
    memcpy(SELF->m_calibration, c, 2 * sizeof(IfxRsp_AntennaCalibration));
    return E_SUCCESS;
}

void ModuleRadar_Constructor(ModuleRadar *self, IfxRfe_MmicConfig *mmicConfig, IfxRfe_Sequence *sequence,
                             IfxRsp_Stages *processingStages, IfxRsp_AntennaCalibration *calibration, const LedDefinition_t *ledMeasuring, const LedDefinition_t *ledTransferring)
{
    Module_Constructor(&self->base, (IModule *)(uintptr_t)self, ledMeasuring, ledTransferring);
    self->b_IModuleRadar.b_IModule.registerDataCallback = ModuleRadar_registerDataCallback;

    self->b_IModuleRadar.getDataProperties   = ModuleRadar_getDataProperties;
    self->b_IModuleRadar.getRadarInfo        = ModuleRadar_getRadarInfo;
    self->b_IModuleRadar.getConfiguration    = ModuleRadar_getConfiguration;
    self->b_IModuleRadar.getSequence         = ModuleRadar_getSequence;
    self->b_IModuleRadar.getProcessingStages = ModuleRadar_getProcessingStages;
    self->b_IModuleRadar.getCalibration      = ModuleRadar_getCalibration;
    self->b_IModuleRadar.setConfiguration    = ModuleRadar_setConfiguration;
    self->b_IModuleRadar.setSequence         = ModuleRadar_setSequence;
    self->b_IModuleRadar.setProcessingStages = ModuleRadar_setProcessingStages;
    self->b_IModuleRadar.setCalibration      = ModuleRadar_setCalibration;

    self->m_mmicConfig       = mmicConfig;
    self->m_sequence         = sequence;
    self->m_processingStages = processingStages;
    self->m_calibration      = calibration;
}
