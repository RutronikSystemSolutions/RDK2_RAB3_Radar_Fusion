/**
 *
 */
#ifndef REMOTE_MODULE_RADAR_H
#define REMOTE_MODULE_RADAR_H 1

#include "RemoteProcessingRadar.h"
#include "RemoteRadar.h"
#include "RemoteRadarRxs.h"
#include <modules/interfaces/IModuleRadar.h>
#include <platform/interfaces/IVendorCommands.h>
#include <stdint.h>

#define REMOTE_RADAR_MAX_COUNT (2)


typedef struct _RemoteModuleRadar RemoteModuleRadar;

struct _RemoteModuleRadar
{
    IModuleRadar b_IModuleRadar;
    IVendorCommands *m_vendorCommands;

    uint8_t m_id;
    uint8_t m_radarCount;
    uint8_t m_processingCount;

    //    RemoteRadar m_radar[REMOTE_RADAR_MAX_COUNT];
    RemoteRadarRxs m_radarRxs[REMOTE_RADAR_MAX_COUNT];
    RemoteProcessingRadar m_processing[REMOTE_RADAR_MAX_COUNT];
};

void RemoteModuleRadar_Constructor(RemoteModuleRadar *self, IVendorCommands *vendorCommands, uint8_t index, uint8_t radarCount, uint8_t radarStartId, uint8_t processingCount, uint8_t processingStartId);

#endif /* REMOTE_MODULE_RADAR_H */
