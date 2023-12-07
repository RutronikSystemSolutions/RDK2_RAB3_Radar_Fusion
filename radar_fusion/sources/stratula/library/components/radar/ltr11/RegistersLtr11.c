#include "RegistersLtr11.h"


#define SELF     ((RegistersLtr11 *)(uintptr_t)self)
#define PROTOCOL (SELF->m_protocol)

/*
 * The transmit data consists of:
 *  |  bits[23:17]  |  16  |  bits[15:0]  |
 *  |---------------|------|--------------|
 *  |    ADDRESS    | ~r/w |   0x000000   |
 *
 *
 * The receive data consists of:
 *  |  bits[23:16]  |  bits[15:0]  |
 *  |---------------|--------------|
 *  |      GSR      |     DATA     |
 */

static sr_t RegistersLtr11_read(IRegisters8_16 *self, AddrType regAddr, RegType *value)
{
    const uint8_t command = LTR11_READ(regAddr);

    return PROTOCOL->executeRead(PROTOCOL, command, value);
}

static sr_t RegistersLtr11_readBurst(IRegisters8_16 *self, AddrType regAddr, AddrType count, RegType *values)
{
    const uint8_t command[2] = LTR11_READ_BURST(regAddr);

    return PROTOCOL->executeReadBurst(PROTOCOL, command, count, values);
}

static sr_t RegistersLtr11_write(IRegisters8_16 *self, AddrType regAddr, RegType value)
{
    const uint8_t command[3] = LTR11_WRITE(regAddr, value);

    return PROTOCOL->executeWrite(PROTOCOL, command);
}

static sr_t RegistersLtr11_writeBurst(IRegisters8_16 *self, AddrType regAddr, AddrType count, const RegType values[])
{
    const uint8_t command[2] = LTR11_WRITE_BURST(regAddr);
    uint8_t buf[count][2];
    for (uint16_t i = 0; i < count; i++)
    {
        const uint8_t writeValue[2] = LTR11_WRITE_VALUE(values[i]);
        buf[i][0]                   = writeValue[0];
        buf[i][1]                   = writeValue[1];
    }

    return PROTOCOL->executeWriteBurst(PROTOCOL, command, count, (const uint8_t(*)[2])buf);
}

void RegistersLtr11_Constructor(RegistersLtr11 *self, IProtocolLtr11 *protocol)
{
    Registers8_16_Constructor(&self->b_Registers, 1);

    self->b_Registers.b_IRegisters.read       = RegistersLtr11_read;
    self->b_Registers.b_IRegisters.write      = RegistersLtr11_write;
    self->b_Registers.b_IRegisters.readBurst  = RegistersLtr11_readBurst;
    self->b_Registers.b_IRegisters.writeBurst = RegistersLtr11_writeBurst;

    {
        self->m_protocol = protocol;
    }
}
