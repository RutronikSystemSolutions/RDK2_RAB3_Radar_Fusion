/**
 *
 */
#ifndef REMOTE_REGISTERS_16
#define REMOTE_REGISTERS_16 1

#include <components/interfaces/IRegisters16.h>
#include <platform/interfaces/IVendorCommands.h>
#include <stdint.h>

typedef struct _RemoteRegisters16 RemoteRegisters16;

struct _RemoteRegisters16
{
    IRegisters16 b_IRegisters;

    IVendorCommands *m_vendorCommands;
    uint8_t m_id;
};

void RemoteRegisters16_Constructor(RemoteRegisters16 *self, IVendorCommands *vendorCommands, uint8_t id);

#endif /* REMOTE_REGISTERS_16 */
