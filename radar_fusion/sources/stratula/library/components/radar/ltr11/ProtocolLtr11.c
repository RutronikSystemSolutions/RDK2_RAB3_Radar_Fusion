#include "ProtocolLtr11.h"

#include <common/endian_conversion.h>
#include <fatal_error.h>
#include <stddef.h>

#define SELF ((ProtocolLtr11 *)(uintptr_t)self)

#define MISO_GUARD_TIME      timer_microseconds(130)  // guard time as 80 us + pulse 53us
#define MISO_MAX_BURST_COUNT 4

sr_t ProtocolLtr11_executeWrite(IProtocolLtr11 *self, const uint8_t command[3])
{
    while (is_locked(&SELF->m_misoLock))
        ;
    lock(&SELF->m_spiLock);
    const sr_t ret = SELF->m_accessSpi->write8(SELF->m_devId, 3, command, false);
    unlock(&SELF->m_spiLock);

    return ret;
}

sr_t ProtocolLtr11_executeRead(IProtocolLtr11 *self, uint8_t command, uint16_t *value)
{
    uint8_t *buf = (uint8_t *)(uintptr_t)value;

    while (is_locked(&SELF->m_misoLock))
        ;
    lock(&SELF->m_spiLock);
    sr_t ret = SELF->m_accessSpi->write8(SELF->m_devId, sizeof(command), &command, true);
    if (ret == E_SUCCESS)
    {
        ret = SELF->m_accessSpi->read8(SELF->m_devId, sizeof(*value), buf, false);
    }
    unlock(&SELF->m_spiLock);
    big_to_cpu_16(value);

    return ret;
}

sr_t ProtocolLtr11_setBits(IProtocolLtr11 *self, uint8_t address, uint16_t bitMask)
{
    uint16_t value;
    const uint8_t read_command = LTR11_READ(address);
    RETURN_ON_ERROR(self->executeRead(self, read_command, &value));
    value |= bitMask;
    const uint8_t write_command[3] = LTR11_WRITE(address, value);
    RETURN_ON_ERROR(self->executeWrite(self, write_command));

    return E_SUCCESS;
}

sr_t ProtocolLtr11_executeWriteBatch(IProtocolLtr11 *self, const uint8_t commands[][3], uint16_t count)
{
    while (count--)
    {
        RETURN_ON_ERROR(ProtocolLtr11_executeWrite(self, *commands++));
    }

    return E_SUCCESS;
}

sr_t ProtocolLtr11_executeWriteBurst(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, const uint8_t values[][2])
{
    if ((SELF->m_misoArbitration) && (count > MISO_MAX_BURST_COUNT))
    {
        return E_NOT_AVAILABLE;
    }

    const uint32_t length = count * sizeof(values[0]);

    while (is_locked(&SELF->m_misoLock))
        ;
    lock(&SELF->m_spiLock);
    sr_t ret = SELF->m_accessSpi->write8(SELF->m_devId, 2, command, true);
    if (ret == E_SUCCESS)
    {
        ret = SELF->m_accessSpi->write8(SELF->m_devId, length, *values, false);
    }
    unlock(&SELF->m_spiLock);

    return ret;
}

sr_t ProtocolLtr11_executeReadBurst(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, uint16_t values[])
{
    if ((SELF->m_misoArbitration) && (count > MISO_MAX_BURST_COUNT))
    {
        return E_NOT_AVAILABLE;
    }

    const uint32_t length = count * sizeof(values[0]);
    uint8_t *buf          = (uint8_t *)values;

    while (is_locked(&SELF->m_misoLock))
        ;
    lock(&SELF->m_spiLock);
    sr_t ret = SELF->m_accessSpi->write8(SELF->m_devId, 2, command, true);
    if (ret == E_SUCCESS)
    {
        ret = SELF->m_accessSpi->read8(SELF->m_devId, length, buf, false);
    }
    unlock(&SELF->m_spiLock);
    while (count--)
    {
        big_to_cpu_16(values++);
    }

    return ret;
}

static void PRT_Timer_Callback(void *self)
{
    SELF->m_allowSpiAccess = false;
    lock(&SELF->m_misoLock);
    Timer_stop(SELF->m_timerId);
}

static void ProtocolLtr11_irqFallingEdge(void *self)
{
    // check if SPI transactions are allowed (MISO arbitration is disabled, or an SPI transaction is active)
    if ((SELF->m_misoArbitration == false) || (SELF->m_allowSpiAccess == true))
    {
        return;
    }

    SELF->m_allowSpiAccess = true;
    Timer_start(SELF->m_timerId, SELF->m_interval);
    unlock(&SELF->m_misoLock);

    ReadoutCallback readoutCallback = SELF->m_callback;
    if (readoutCallback)
    {
        readoutCallback(SELF->m_callbackArg);
    }
}

sr_t ProtocolLtr11_registerCallback(IProtocolLtr11 *self, ReadoutCallback callback, void *arg)
{
    SELF->m_callback    = callback;
    SELF->m_callbackArg = arg;
    return E_SUCCESS;
}

sr_t ProtocolLtr11_setMisoArbitration(IProtocolLtr11 *self, uint16_t prt)
{
    if ((prt != 0) && (prt <= MISO_GUARD_TIME))
    {
        return E_INVALID_PARAMETER;
    }

    // check if MISO arbitration is enabled and needs to be disabled
    if (SELF->m_misoArbitration)
    {
        SELF->m_misoArbitration = false;
        // if m_misoArbitration was true, and m_allowSpiAccess is true, the timer is running
        // in this case, wait until its isr is serviced and sets m_allowSpiAccess to false
        while (SELF->m_allowSpiAccess)
            ;
        // the timer is not running at this point, so we can unlock m_misoLock
        unlock(&SELF->m_misoLock);

        PlatformInterrupt_enable(SELF->m_irqConfig, false);  // disable IRQ
    }

    if (prt != 0)
    {
        SELF->m_interval = timer_microseconds(prt) - MISO_GUARD_TIME;

        // check if MISO arbitration is disabled and needs to be enabled
        if (!SELF->m_misoArbitration)
        {
            SELF->m_misoArbitration = true;
            lock(&SELF->m_misoLock);

            PlatformInterrupt_enable(SELF->m_irqConfig, true);  // enable IRQ
        }
    }
    SELF->m_allowSpiAccess = !SELF->m_misoArbitration;

    return E_SUCCESS;
}

void ProtocolLtr11_Constructor(ProtocolLtr11 *self, ISpi *accessSpi, uint8_t devId, const PlatformInterruptDefinition_t *irqConfig)
{
    self->b_IProtocolLtr11.executeRead        = ProtocolLtr11_executeRead;
    self->b_IProtocolLtr11.executeWrite       = ProtocolLtr11_executeWrite;
    self->b_IProtocolLtr11.setBits            = ProtocolLtr11_setBits;
    self->b_IProtocolLtr11.executeWriteBatch  = ProtocolLtr11_executeWriteBatch;
    self->b_IProtocolLtr11.executeReadBurst   = ProtocolLtr11_executeReadBurst;
    self->b_IProtocolLtr11.executeWriteBurst  = ProtocolLtr11_executeWriteBurst;
    self->b_IProtocolLtr11.setMisoArbitration = ProtocolLtr11_setMisoArbitration;
    self->b_IProtocolLtr11.registerCallback   = ProtocolLtr11_registerCallback;

    {
        self->m_accessSpi       = accessSpi;
        self->m_devId           = devId;
        self->m_irqConfig       = irqConfig;
        self->m_callback        = NULL;
        self->m_callbackArg     = NULL;
        self->m_misoArbitration = false;
        self->m_allowSpiAccess  = true;

        reset_lock(&self->m_spiLock);
        reset_lock(&self->m_misoLock);

        const uint8_t flags    = SPI_MODE_3;  ///< SPI mode/configuration flags
        const uint8_t wordSize = 8;           ///< number of bits per transaction
        const uint32_t speed   = 50000000;    ///< device speed
        if (self->m_accessSpi->configure(self->m_devId, flags, wordSize, speed) != E_SUCCESS)
        {
            fatal_error(FATAL_ERROR_SPI_CONFIG_FAILED);
        }

        if (irqConfig != NULL)
        {
            self->m_timerId = Timer_add();
            if (Timer_setCallback(self->m_timerId, PRT_Timer_Callback, self) != E_SUCCESS)
            {
                fatal_error(0);
            }

            if (PlatformInterrupt_registerCallback(self->m_irqConfig, ProtocolLtr11_irqFallingEdge, SELF) != E_SUCCESS)
            {
                fatal_error(0);
            }
        }
    }
}
