/**
 *
 */
#ifndef REMOTE_PINS_RXS
#define REMOTE_PINS_RXS 1

#include <components/interfaces/IPinsRxs.h>
#include <platform/interfaces/IVendorCommands.h>


typedef struct _RemotePinsRxs RemotePinsRxs;

struct _RemotePinsRxs
{
    IPinsRxs b_IPinsRxs;

    IVendorCommands *m_vendorCommands;
    uint8_t m_id;
};

void RemotePinsRxs_Constructor(RemotePinsRxs *self, IVendorCommands *vendorCommands, uint8_t id);

#endif /* REMOTE_PINS_RXS */
