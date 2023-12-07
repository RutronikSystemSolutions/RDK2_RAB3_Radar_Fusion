#include "ProtocolAvian.h"

#include <fatal_error.h>
#include <stddef.h>


#define SELF ((ProtocolAvian *)(uintptr_t)self)


sr_t ProtocolAvian_execute(IProtocolAvian *self, const uint8_t commands[][4], uint32_t count, uint32_t results[])
{
    while (count--)
    {
        uint8_t bufRead[4];

        RETURN_ON_ERROR(SELF->m_accessSpi->transfer8(SELF->m_devId, sizeof(*commands), *commands++, bufRead, false));

        if (results)
        {
            *results++ = (bufRead[0] << 24) | (bufRead[1] << 16) | (bufRead[2] << 8) | bufRead[3];
        }
    }

    return E_SUCCESS;
}

sr_t ProtocolAvian_setBits(IProtocolAvian *self, uint8_t address, uint32_t bitMask)
{
    uint8_t command[4] = {
        address << I_PROTOCOL_AVIAN_ADDRESS_OFFSET,
        0u,
        0u,
        0u,
    };
    uint32_t value;
    RETURN_ON_ERROR(ProtocolAvian_execute(self, (const uint8_t(*)[4])command, 1, &value));

    value |= bitMask;
    command[0] |= I_PROTOCOL_AVIAN_WRITE_BIT;
    command[1] = value >> 16;
    command[2] = value >> 8;
    command[3] = value & 0xFF;
    return ProtocolAvian_execute(self, (const uint8_t(*)[4])command, 1, NULL);
}

void ProtocolAvian_Constructor(ProtocolAvian *self, ISpi *accessSpi, uint8_t devId)
{
    self->b_IProtocolAvian.execute = ProtocolAvian_execute;
    self->b_IProtocolAvian.setBits = ProtocolAvian_setBits;

    {
        self->m_accessSpi = accessSpi;
        self->m_devId     = devId;

        const uint8_t flags = SPI_MODE_0;  ///< SPI mode/configuration flags

        const uint8_t wordSize = 8;  ///< number of bits per transaction

        const uint32_t speed = 10000000;  ///< device speed
        if (self->m_accessSpi->configure(self->m_devId, flags, wordSize, speed) != E_SUCCESS)
        {
            fatal_error(FATAL_ERROR_SPI_CONFIG_FAILED);
        }
    }
}
