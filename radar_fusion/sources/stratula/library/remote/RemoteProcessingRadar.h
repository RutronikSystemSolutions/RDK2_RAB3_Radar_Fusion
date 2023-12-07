/**
 *
 */
#ifndef REMOTEPROCESSINGRADAR_H
#define REMOTEPROCESSINGRADAR_H 1

#include <platform/interfaces/IProcessingRadar.h>
#include <platform/interfaces/IVendorCommands.h>
#include <stdint.h>


typedef struct _RemoteProcessingRadar RemoteProcessingRadar;

struct _RemoteProcessingRadar
{
    IProcessingRadar b_IProcessingRadar;

    IVendorCommands *m_vendorCommands;
    uint8_t m_id;
    IfxRsp_Signal m_rawData;
};

void RemoteProcessingRadar_Constructor(RemoteProcessingRadar *self, IVendorCommands *vendorCommands, uint8_t id);

#endif /* REMOTEPROCESSINGRADAR_H */
