#include "RemoteRegisters16.h"

#include <common/serialization.h>
#include <stddef.h>
#include <universal/components/implementations/radar.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/subinterfaces/iregisters.h>
#include <universal/components/types.h>
#include <universal/protocol/protocol_definitions.h>

#define SELF ((RemoteRegisters16 *)(uintptr_t)self)


static sr_t RemoteRegisters16_vendorWrite(RemoteRegisters16 *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_REGISTERS, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRegisters16_vendorTransfer(RemoteRegisters16 *self, uint8_t bFunction, uint16_t sizeIn, const uint8_t *bufIn, uint16_t *sizeOut, uint8_t **bufOut)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_REGISTERS, bFunction);
    return self->m_vendorCommands->vendorTransfer(CMD_COMPONENT, wValue, wIndex, sizeIn, bufIn, sizeOut, bufOut);
}

static sr_t RemoteRegisters16_readBurst(IRegisters16 *self, uint16_t regAddr, uint16_t count, uint16_t values[])
{
    uint8_t bufIn[4];
    hostToSerial16(bufIn, regAddr);
    hostToSerial16(bufIn + sizeof(regAddr), count);
    uint16_t sizeOut = count * sizeof(values[0]);
    uint8_t *bufOut  = (uint8_t *)values;

    RETURN_ON_ERROR(RemoteRegisters16_vendorTransfer(SELF, FN_REGISTERS_READ_BURST, 4, bufIn, &sizeOut, &bufOut));

    if (sizeOut != count * sizeof(values[0]))
    {
        return E_UNEXPECTED_VALUE;
    }
    if (bufOut != (uint8_t *)values)
    {
        memcpy(values, bufOut, sizeOut);
    }

    return E_SUCCESS;
}

static sr_t RemoteRegisters16_writeBurst(IRegisters16 *self, uint16_t regAddr, uint16_t count, const uint16_t values[])
{
    const uint16_t length  = count * sizeof(values[0]);
    const uint16_t argSize = sizeof(regAddr);
    uint8_t buf[length + argSize];
    memcpy(buf, values, length);  // todo: use proper function
    hostToSerial16(buf + length, regAddr);

    return RemoteRegisters16_vendorWrite(SELF, FN_REGISTERS_WRITE_BURST, argSize + length, buf);
}

static sr_t RemoteRegisters16_read(IRegisters16 *self, uint16_t regAddr, uint16_t *readData)
{
    return RemoteRegisters16_readBurst(self, regAddr, 1, readData);
}

static sr_t RemoteRegisters16_write(IRegisters16 *self, uint16_t regAddr, uint16_t value)
{
    return RemoteRegisters16_writeBurst(self, regAddr, 1, &value);
}

static sr_t RemoteRegisters16_readBatch(IRegisters16 *self, const uint16_t regAddr[], uint16_t count, uint16_t values[])
{
    const uint16_t length = count * sizeof(regAddr[0]);
    uint8_t bufIn[length];
    memcpy(bufIn, regAddr, length);  // todo: use proper function
    uint16_t sizeOut = count * sizeof(values[0]);
    uint8_t *bufOut  = (uint8_t *)values;

    RETURN_ON_ERROR(RemoteRegisters16_vendorTransfer(SELF, FN_REGISTERS_BATCH, count, bufIn, &sizeOut, &bufOut));

    if (sizeOut != count * sizeof(values[0]))
    {
        return E_UNEXPECTED_VALUE;
    }
    if (bufOut != (uint8_t *)values)
    {
        memcpy(values, bufOut, sizeOut);
    }

    return E_SUCCESS;
}

static sr_t RemoteRegisters16_writeBatch(IRegisters16 *self, const BatchType regVals[], uint16_t count)
{
    const uint16_t length = count * sizeof(regVals[0]);
    uint8_t buf[length];
    memcpy(buf, regVals, length);  // todo: use proper function

    return RemoteRegisters16_vendorWrite(SELF, FN_REGISTERS_BATCH, length, buf);
}

static sr_t RemoteRegisters16_writeBatchArray(IRegisters16 *self, const uint16_t regVals[][2], uint16_t count)
{
    return RemoteRegisters16_writeBatch(self, (const BatchType *)regVals, count);
}

static sr_t RemoteRegisters16_setBits(IRegisters16 *self, uint16_t regAddr, uint16_t bitmask)
{
    const uint8_t size = sizeof(regAddr) + sizeof(bitmask);
    uint8_t buf[size];
    hostToSerial16(buf, regAddr);
    hostToSerial16(buf + sizeof(regAddr), bitmask);

    return RemoteRegisters16_vendorWrite(SELF, FN_REGISTERS_SET_BITS, size, buf);
}

static sr_t RemoteRegisters16_clearBits(IRegisters16 *self, uint16_t regAddr, uint16_t bitmask)
{
    const uint8_t size = sizeof(regAddr) + sizeof(bitmask);
    uint8_t buf[size];
    hostToSerial16(buf, regAddr);
    hostToSerial16(buf + sizeof(regAddr), bitmask);

    return RemoteRegisters16_vendorWrite(SELF, FN_REGISTERS_CLEAR_BITS, size, buf);
}

static sr_t RemoteRegisters16_modifyBits(IRegisters16 *self, uint16_t regAddr, uint16_t setBitmask, uint16_t clearBitmask)
{
    const uint8_t size = sizeof(regAddr) + sizeof(setBitmask) + sizeof(setBitmask);
    uint8_t buf[size];
    hostToSerial16(buf, regAddr);
    hostToSerial16(buf + sizeof(regAddr), clearBitmask);
    hostToSerial16(buf + sizeof(regAddr) + sizeof(clearBitmask), setBitmask);

    return RemoteRegisters16_vendorWrite(SELF, FN_REGISTERS_MODIFY_BITS, size, buf);
}

void RemoteRegisters16_Constructor(RemoteRegisters16 *self, IVendorCommands *vendorCommands, uint8_t id)
{
    self->m_vendorCommands = vendorCommands;
    self->m_id             = id;

    self->b_IRegisters.read            = RemoteRegisters16_read;
    self->b_IRegisters.write           = RemoteRegisters16_write;
    self->b_IRegisters.readBurst       = RemoteRegisters16_readBurst;
    self->b_IRegisters.writeBurst      = RemoteRegisters16_writeBurst;
    self->b_IRegisters.writeBatch      = RemoteRegisters16_writeBatch;
    self->b_IRegisters.writeBatchArray = RemoteRegisters16_writeBatchArray;
    self->b_IRegisters.readBatch       = RemoteRegisters16_readBatch;
    self->b_IRegisters.setBits         = RemoteRegisters16_setBits;
    self->b_IRegisters.clearBits       = RemoteRegisters16_clearBits;
    self->b_IRegisters.modifyBits      = RemoteRegisters16_modifyBits;
}
