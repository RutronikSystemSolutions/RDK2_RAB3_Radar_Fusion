/**
 *
 */
#ifndef REMOTE_RADARRXS
#define REMOTE_RADAR_RXS 1

#include "RemotePinsRxs.h"
#include "RemoteRadar.h"
#include "RemoteRegisters16.h"
#include <components/interfaces/IRadarRxs.h>
#include <platform/interfaces/IVendorCommands.h>
#include <stdint.h>

typedef struct _RemoteRadarRxs RemoteRadarRxs;

struct _RemoteRadarRxs
{
    IRadarRxs b_IRadarRxs;
    IVendorCommands *m_vendorCommands;
    uint8_t m_id;

    RemotePinsRxs m_radarPins;
    RemoteRegisters16 m_radarRegisters;
};

void RemoteRadarRxs_Constructor(RemoteRadarRxs *self, IVendorCommands *vendorCommands, uint8_t id);

#endif /* REMOTE_RADAR_RXS */
