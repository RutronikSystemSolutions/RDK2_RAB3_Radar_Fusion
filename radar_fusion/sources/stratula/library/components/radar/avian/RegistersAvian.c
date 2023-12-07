#include "RegistersAvian.h"


#define SELF     ((RegistersAvian *)(uintptr_t)self)
#define PROTOCOL (SELF->m_protocol)

#define SPI_CLK_NUM_ERR 0x01
#define SPI_BURST_ERR   0x02
#define SPI_ERRORS      ((SPI_CLK_NUM_ERR | SPI_BURST_ERR) << 24)

/*
 * The transmit data consists of:
 *  |  bits[31:25]  |  24  |  bits[23:0]  |
 *  |---------------|------|--------------|
 *  |    ADDRESS    | ~r/w |   0x000000   |
 *
 *
 * The receive data consists of:
 *  |  bits[31:24]  |  bits[23:0]  |
 *  |---------------|--------------|
 *  |      GSR      |     DATA     |
 */

static sr_t RegistersAvian_read(IRegisters8_32 *self, AddrType regAddr, RegType *value)
{
    if (regAddr & 0x80)
    {
        //throw EAdapter("Invalid register address", regAddr);
        return E_INVALID_PARAMETER;
    }

    const uint8_t command[4] = AVIAN_READ(regAddr);
    RETURN_ON_ERROR(PROTOCOL->execute(PROTOCOL, &command, 1, value));

    if (*value & SPI_ERRORS)
    {
        return E_UNEXPECTED_VALUE;
    }

    *value &= 0x00FFFFFF;
    return E_SUCCESS;
}

static sr_t RegistersAvian_write(IRegisters8_32 *self, AddrType regAddr, RegType value)
{
    if (regAddr & 0x80)
    {
        return E_INVALID_PARAMETER;
    }

    if (value & 0xFF000000)
    {
        return E_INVALID_PARAMETER;
    }

    const uint8_t command[4] = AVIAN_WRITE(regAddr, value);
    uint32_t result;
    RETURN_ON_ERROR(PROTOCOL->execute(PROTOCOL, &command, 1, &result));

    if (result & SPI_ERRORS)
    {
        return E_UNEXPECTED_VALUE;
    }
    return E_SUCCESS;
}

void RegistersAvian_Constructor(RegistersAvian *self, IProtocolAvian *protocol)
{
    Registers8_32_Constructor(&self->b_Registers, 1);

    self->b_Registers.b_IRegisters.read  = RegistersAvian_read;
    self->b_Registers.b_IRegisters.write = RegistersAvian_write;

    {
        self->m_protocol = protocol;
    }
}
