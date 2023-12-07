/**
 * \addtogroup        RadarModuleInterface
 * @{
 */
#ifndef I_MODULE_RADAR_H
#define I_MODULE_RADAR_H 1

#include <modules/interfaces/IModule.h>
#include <platform/interfaces/IProcessingRadar.h>
#include <universal/data_definitions.h>
#include <universal/types/IfxRfe_Types.h>
#include <universal/types/IfxRsp_Types.h>

typedef struct _IModuleRadar IModuleRadar;

struct _IModuleRadar
{
    IModule b_IModule;

    /**
     * Get measurement input information
     *
     * @param info pointer to the memory structure where to store the information
     * @return Strata error code
     */
    sr_t (*getDataProperties)(IModuleRadar *self, IDataProperties_t *props);
    sr_t (*getRadarInfo)(IModuleRadar *self, IProcessingRadarInput_t *info, const IDataProperties_t *dataProperties);


    IfxRfe_MmicConfig *(*getConfiguration)(IModuleRadar *self);
    IfxRfe_Sequence *(*getSequence)(IModuleRadar *self);
    IfxRsp_Stages *(*getProcessingStages)(IModuleRadar *self);
    IfxRsp_AntennaCalibration *(*getCalibration)(IModuleRadar *self);

    sr_t (*setConfiguration)(IModuleRadar *self, const IfxRfe_MmicConfig *c);
    sr_t (*setSequence)(IModuleRadar *self, const IfxRfe_Sequence *s);
    sr_t (*setProcessingStages)(IModuleRadar *self, const IfxRsp_Stages *s);
    sr_t (*setCalibration)(IModuleRadar *self, const IfxRsp_AntennaCalibration c[2]);
};


#endif /* I_MODULE_RADAR_H */
/** @} */
