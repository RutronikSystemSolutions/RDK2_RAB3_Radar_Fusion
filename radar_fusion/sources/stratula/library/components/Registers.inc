

#define SELF ((RegistersType *)self)


static sr_t RegistersType_readBurst(IRegistersType *self, AddrType regAddr, AddrType count, RegType values[])
{
    while (count--)
    {
        RETURN_ON_ERROR(self->read(self, regAddr, values++));
        regAddr += SELF->m_increment;
    }
    return E_SUCCESS;
}

static sr_t RegistersType_writeBurst(IRegistersType *self, AddrType regAddr, AddrType count, const RegType values[])
{
    while (count--)
    {
        RETURN_ON_ERROR(self->write(self, regAddr, *values++));
        regAddr += SELF->m_increment;
    }
    return E_SUCCESS;
}

static sr_t RegistersType_writeBatch(IRegistersType *self, const BatchType values[], AddrType count)
{
    while (count--)
    {
        RETURN_ON_ERROR(self->write(self, values->address, values->value));
        values++;
    }
    return E_SUCCESS;
}

#ifndef IRegisters_Extended
static sr_t RegistersType_writeBatchArray(IRegistersType *self, const RegType regVals[][2], AddrType count)
{
    while (count--)
    {
        RETURN_ON_ERROR(self->write(self, (*regVals)[0], (*regVals)[1]));
        regVals++;
    }
    return E_SUCCESS;
}
#endif

static sr_t RegistersType_readBatch(IRegistersType *self, const AddrType regAddr[], AddrType count, RegType values[])
{
    while (count--)
    {
        RETURN_ON_ERROR(self->read(self, *regAddr++, values++));
    }
    return E_SUCCESS;
}

static sr_t RegistersType_setBits(IRegistersType *self, AddrType regAddr, RegType bitmask)
{
    RegType value;
    RETURN_ON_ERROR(self->read(self, regAddr, &value));

    value |= bitmask;

    return self->write(self, regAddr, value);
}

static sr_t RegistersType_clearBits(IRegistersType *self, AddrType regAddr, RegType bitmask)
{
    RegType value;
    RETURN_ON_ERROR(self->read(self, regAddr, &value));

    value &= ~bitmask;

    return self->write(self, regAddr, value);
}

static sr_t RegistersType_modifyBits(IRegistersType *self, AddrType regAddr, RegType clearBitmask, RegType setBitmask)
{
    RegType value;
    RETURN_ON_ERROR(self->read(self, regAddr, &value));

    value &= ~clearBitmask;
    value |= setBitmask;

    return self->write(self, regAddr, value);
}

void COMBINE(RegistersType, _, Constructor)(RegistersType *self, RegType increment)
{
    self->b_IRegisters.readBurst  = RegistersType_readBurst;
    self->b_IRegisters.writeBurst = RegistersType_writeBurst;
    self->b_IRegisters.writeBatch = RegistersType_writeBatch;
    self->b_IRegisters.readBatch  = RegistersType_readBatch;

#ifndef IRegisters_Extended
    self->b_IRegisters.writeBatchArray = RegistersType_writeBatchArray;
#endif

    self->b_IRegisters.setBits    = RegistersType_setBits;
    self->b_IRegisters.clearBits  = RegistersType_clearBits;
    self->b_IRegisters.modifyBits = RegistersType_modifyBits;

    {
        self->m_increment = increment;
    }
}
