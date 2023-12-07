#include "ProtocolAtr22.h"

#include <common/endian_conversion.h>
#include <fatal_error.h>
#include <stddef.h>


#define SELF ((ProtocolAtr22 *)(uintptr_t)self)


sr_t ProtocolAtr22_executeWrite(IProtocolAtr22 *self, const uint8_t commands[][4], uint16_t count)
{
    const uint16_t length = count * sizeof(*commands);
    const uint8_t *buf    = commands[0];

    return SELF->m_accessI2c->writeWithoutPrefix(SELF->m_devAddr, length, buf);
}

sr_t ProtocolAtr22_executeRead(IProtocolAtr22 *self, uint16_t command, uint16_t count, uint16_t values[])
{
    const uint16_t length = count * sizeof(*values);
    uint8_t *buf          = (uint8_t *)values;

    RETURN_ON_ERROR(SELF->m_accessI2c->readWith16BitPrefix(SELF->m_devAddr, command, length, buf));

    while (count--)
    {
        big_to_cpu_16(values++);
    }

    return E_SUCCESS;
}

sr_t ProtocolAtr22_setBits(IProtocolAtr22 *self, uint16_t address, uint16_t bitMask)
{
    uint16_t value;
    const uint16_t read_command = ATR22_READ(address);
    RETURN_ON_ERROR(self->executeRead(self, read_command, 1, &value));
    value |= bitMask;
    const uint8_t write_command[4] = ATR22_WRITE(address, value);
    RETURN_ON_ERROR(self->executeWrite(self, &write_command, 1));
    return E_SUCCESS;
}

void ProtocolAtr22_Constructor(ProtocolAtr22 *self, II2c *accessI2c, uint16_t devAddr)
{
    self->b_IProtocolAtr22.executeWrite = ProtocolAtr22_executeWrite;
    self->b_IProtocolAtr22.executeRead  = ProtocolAtr22_executeRead;
    self->b_IProtocolAtr22.setBits      = ProtocolAtr22_setBits;

    {
        self->m_accessI2c = accessI2c;
        self->m_devAddr   = devAddr;
    }
}
