#include "RegistersAtr22.h"


#define SELF     ((RegistersAtr22 *)(uintptr_t)self)
#define PROTOCOL (SELF->m_protocol)


static sr_t RegistersAtr22_read(IRegisters16 *self, uint16_t regAddr, uint16_t *value)
{
    const uint16_t command = ATR22_READ(regAddr);
    const uint16_t count   = 1;

    return PROTOCOL->executeRead(PROTOCOL, command, count, value);
}

static sr_t RegistersAtr22_write(IRegisters16 *self, uint16_t regAddr, uint16_t value)
{
    const uint8_t command[4] = ATR22_WRITE(regAddr, value);

    return PROTOCOL->executeWrite(PROTOCOL, &command, 1);
}

static sr_t RegistersAtr22_readBurst(IRegistersType *self, uint16_t regAddr, uint16_t count, uint16_t values[])
{
    const uint16_t command = ATR22_READ(regAddr);

    return PROTOCOL->executeRead(PROTOCOL, command, count, values);
}

void RegistersAtr22_Constructor(RegistersAtr22 *self, IProtocolAtr22 *protocol)
{
    Registers16_Constructor(&self->b_Registers, 1);

    self->b_Registers.b_IRegisters.read  = RegistersAtr22_read;
    self->b_Registers.b_IRegisters.write = RegistersAtr22_write;

    self->b_Registers.b_IRegisters.readBurst = RegistersAtr22_readBurst;

    {
        self->m_protocol = protocol;
    }
}
