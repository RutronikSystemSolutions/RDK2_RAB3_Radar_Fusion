#ifndef MODULE_RADAR_H
#define MODULE_RADAR_H 1

#include <modules/Module.h>
#include <modules/interfaces/IModuleRadar.h>


#define MODULE       (&SELF->base.base)
#define MODULE_RADAR ((IModuleRadar *)(uintptr_t)self)


typedef struct _ModuleRadar ModuleRadar;


void ModuleRadar_Constructor(ModuleRadar *self, IfxRfe_MmicConfig *mmicConfig, IfxRfe_Sequence *sequence,
                             IfxRsp_Stages *processingStages, IfxRsp_AntennaCalibration *calibration, const LedDefinition_t *ledMeasuring, const LedDefinition_t *ledTransferring);

sr_t ModuleRadar_setConfiguration(IModuleRadar *self, const IfxRfe_MmicConfig *c);
sr_t ModuleRadar_setSequence(IModuleRadar *self, const IfxRfe_Sequence *s);
sr_t ModuleRadar_setProcessingStages(IModuleRadar *self, const IfxRsp_Stages *s);
sr_t ModuleRadar_setCalibration(IModuleRadar *self, const IfxRsp_AntennaCalibration c[2]);


struct _ModuleRadar
{
    IModuleRadar b_IModuleRadar;

    Module base;

    IfxRfe_MmicConfig *m_mmicConfig;
    IfxRfe_Sequence *m_sequence;
    IfxRsp_Stages *m_processingStages;
    IfxRsp_AntennaCalibration *m_calibration;
};


#endif /* MODULE_RADAR_H */
