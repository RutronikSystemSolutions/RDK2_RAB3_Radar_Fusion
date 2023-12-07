#include "SpiLinker.h"

#include <fatal_error.h>
#include <impl/thread.h>
#include <stdbool.h>
#include <stddef.h>


/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/
#define SPILINKER_DE_SELECT_SLAVES 0xA6
#define SPILINKER_READ_BUFFER      0xFF

#define SPILINKER_MAX_MASTER_COUNT 2u
#define SPILINKER_MAX_DEVICE_COUNT 4u
#define SPILINKER_MIN_DATA_WIDTH   2u
#define SPILINKER_MAX_DATA_WIDTH   32u
#define SPILINKER_MAX_TRANSFER     4u

/******************************************************************************/
/*Interface Methods Declaration ----------------------------------------------*/
/******************************************************************************/
static sr_t SpiLinker_write8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel);
static sr_t SpiLinker_write16(uint8_t devId, uint32_t count, const uint16_t buffer[], bool keepSel);
static sr_t SpiLinker_write32(uint8_t devId, uint32_t count, const uint32_t buffer[], bool keepSel);
static sr_t SpiLinker_read8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel);
static sr_t SpiLinker_read16(uint8_t devId, uint32_t count, uint16_t buffer[], bool keepSel);
static sr_t SpiLinker_read32(uint8_t devId, uint32_t count, uint32_t buffer[], bool keepSel);
static sr_t SpiLinker_transfer8(uint8_t devId, uint32_t count, const uint8_t bufWrite[], uint8_t bufRead[], bool keepSel);
static sr_t SpiLinker_transfer16(uint8_t devId, uint32_t count, const uint16_t bufWrite[], uint16_t bufRead[], bool keepSel);
static sr_t SpiLinker_transfer32(uint8_t devId, uint32_t count, const uint32_t bufWrite[], uint32_t bufRead[], bool keepSel);

static uint32_t SpiLinker_getMaxTransfer(void);
static sr_t SpiLinker_configure(uint8_t devId, uint8_t flags, uint8_t wordSize, uint32_t speed);

/******************************************************************************/
/*Interface Definition -------------------------------------------------------*/
/******************************************************************************/
ISpi SpiLinker = {
    .getMaxTransfer = SpiLinker_getMaxTransfer,
    .configure      = SpiLinker_configure,
    .write8         = SpiLinker_write8,
    .write16        = SpiLinker_write16,
    .write32        = SpiLinker_write32,
    .read8          = SpiLinker_read8,
    .read16         = SpiLinker_read16,
    .read32         = SpiLinker_read32,
    .transfer8      = SpiLinker_transfer8,
    .transfer16     = SpiLinker_transfer16,
    .transfer32     = SpiLinker_transfer32,
};

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
static ISpi *m_accessSpi;
static IGpio *m_accessGpio;
static uint8_t m_numDevices;
static const SpiLinkerConfig_Device *m_devicesCfg;

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

sr_t SpiLinker_write16(uint8_t devId, uint32_t count, const uint16_t buffer[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

sr_t SpiLinker_read16(uint8_t devId, uint32_t count, uint16_t buffer[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

sr_t SpiLinker_write32(uint8_t devId, uint32_t count, const uint32_t buffer[], bool keepSel)
{
    //At the moment only 4-bytes transfers are allowed and MSB first
    if (count > (SPILINKER_MAX_TRANSFER / sizeof(uint32_t)))
    {
        return E_INVALID_PARAMETER;
    }

    uint8_t bufOut[4];
    bufOut[0] = buffer[0] >> 24;
    bufOut[1] = buffer[0] >> 16;
    bufOut[2] = buffer[0] >> 8;
    bufOut[3] = buffer[0];
    return SpiLinker_write8(devId, count * sizeof(uint32_t), bufOut, keepSel);
}

sr_t SpiLinker_read32(uint8_t devId, uint32_t count, uint32_t buffer[], bool keepSel)
{
    //At the moment only 4-bytes transfers are allowed and MSB first
    if (count > (SPILINKER_MAX_TRANSFER / sizeof(uint32_t)))
    {
        return E_INVALID_PARAMETER;
    }

    uint8_t bufIn[4];
    sr_t result = SpiLinker_read8(devId, count * sizeof(uint32_t), bufIn, keepSel);
    buffer[0]   = bufIn[0] << 24 | bufIn[1] << 16 | bufIn[2] << 8 | bufIn[3];
    return result;
}

sr_t SpiLinker_write8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel)
{
    if (devId >= m_numDevices || count > SPILINKER_MAX_TRANSFER)
    {
        return E_INVALID_PARAMETER;
    }

    //spiLinker (local and remote side)
    const SpiLinkerConfig_Master *spiLinker = m_devicesCfg[devId].localMaster;
    const uint8_t remoteMasterSelectCmd     = m_devicesCfg[devId].remoteMaster;
    const uint8_t slaveSelectCmd            = m_devicesCfg[devId].slaveSelect;

    //Select remote device
    RETURN_ON_ERROR(m_accessGpio->setPin(m_devicesCfg[devId].gpioSSel, true));

    // Force RO to toggle low for 1 microsecond
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, false));
    this_thread_sleep_for(chrono_microseconds(1));
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, true));

    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &remoteMasterSelectCmd, keepSel));
    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &slaveSelectCmd, keepSel));

    //Request to write data to remote-device
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, false));
    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, count, buffer, keepSel));
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, true));


    //De-select remote device
    const uint8_t slaveDeSelectCmd = SPILINKER_DE_SELECT_SLAVES;
    //Discard received-data from remote-device
    bool state;
    RETURN_ON_ERROR(m_accessGpio->getPin(spiLinker->gpioBne, &state));
    while (state)
    {
        m_accessSpi->write8(spiLinker->devId, 1, &slaveDeSelectCmd, keepSel);
        RETURN_ON_ERROR(m_accessGpio->getPin(spiLinker->gpioBne, &state));
    }

    return m_accessGpio->setPin(m_devicesCfg[devId].gpioSSel, false);
}

sr_t SpiLinker_read8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel)
{
    if (devId >= m_numDevices)
    {
        return E_INVALID_PARAMETER;
    }

    //spiLinker (local and remote side)
    const SpiLinkerConfig_Master *spiLinker = m_devicesCfg[devId].localMaster;
    const uint8_t remoteMasterSelectCmd     = m_devicesCfg[devId].remoteMaster;
    const uint8_t slaveSelectCmd            = m_devicesCfg[devId].slaveSelect;

    //Select remote device
    RETURN_ON_ERROR(m_accessGpio->setPin(m_devicesCfg[devId].gpioSSel, true));

    // Force RO to toggle low for 1 microsecond
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, false));
    this_thread_sleep_for(chrono_microseconds(1));
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, true));

    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &remoteMasterSelectCmd, keepSel));
    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &slaveSelectCmd, keepSel));

    //Request to read data from remote-device
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, false));
    const uint8_t readBufferCmd = SPILINKER_READ_BUFFER;
    for (uint_fast8_t i = 0; i < count; i++)
    {
        RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &readBufferCmd, keepSel));
    }
    RETURN_ON_ERROR(m_accessGpio->setPin(spiLinker->gpioRo, true));

    //Read received-data from remote-device (which shall come into the SpiLinker's buffer)
    //m_accessSpi->read() send all ones while reading (which is equivalent to write SPILINKER_READ_BUFFER to the linker)
    while (count)
    {
        bool state;
        RETURN_ON_ERROR(m_accessGpio->getPin(spiLinker->gpioBne, &state));
        if (state)
        {
            RETURN_ON_ERROR(m_accessSpi->read8(spiLinker->devId, 1, buffer++, keepSel));
            count--;
        }
    }

    //De-select remote device
    const uint8_t slaveDeSelectCmd = SPILINKER_DE_SELECT_SLAVES;
    RETURN_ON_ERROR(m_accessSpi->write8(spiLinker->devId, 1, &slaveDeSelectCmd, keepSel));
    return m_accessGpio->setPin(m_devicesCfg[devId].gpioSSel, false);
}

sr_t SpiLinker_transfer8(uint8_t devId, uint32_t count, const uint8_t bufWrite[], uint8_t bufRead[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

sr_t SpiLinker_transfer16(uint8_t devId, uint32_t count, const uint16_t bufWrite[], uint16_t bufRead[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

sr_t SpiLinker_transfer32(uint8_t devId, uint32_t count, const uint32_t bufWrite[], uint32_t bufRead[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

uint32_t SpiLinker_getMaxTransfer(void)
{
    return SPILINKER_MAX_TRANSFER;
}

sr_t SpiLinker_configure(uint8_t devId, uint8_t flags, uint8_t wordSize, uint32_t speed)
{
    return E_SUCCESS;  // this is a workaround to allow spiLinker to be used with incomplete implementation
}

/******************************************************************************/
/*Interface Methods Definition -----------------------------------------------*/
/******************************************************************************/
void SpiLinker_Constructor(ISpi *accessSpi, IGpio *accessGpio,
                           const SpiLinkerConfig_Master *masters, uint8_t masterCount,
                           const SpiLinkerConfig_Device *devices, uint8_t deviceCount)
{
    if (masters == NULL || devices == NULL || accessSpi == NULL || accessGpio == NULL ||
        masterCount > SPILINKER_MAX_MASTER_COUNT || deviceCount > SPILINKER_MAX_DEVICE_COUNT)
    {
        fatal_error(0);
    }

    m_accessGpio = accessGpio;
    m_accessSpi  = accessSpi;
    m_numDevices = deviceCount;
    m_devicesCfg = devices;

    for (uint_fast8_t i = 0; i < masterCount; i++)
    {
        m_accessGpio->configurePin(masters[i].gpioBne, GPIO_MODE_INPUT_PULL_DOWN);
        m_accessGpio->configurePin(masters[i].gpioRo, GPIO_MODE_OUTPUT_PUSH_PULL | GPIO_FLAG_OUTPUT_INITIAL_HIGH);
    }

    for (uint_fast8_t i = 0; i < m_numDevices; i++)
    {
        m_accessGpio->configurePin(m_devicesCfg[i].gpioSSel, GPIO_MODE_OUTPUT_PUSH_PULL);

        if (m_devicesCfg[i].config & SPI_LSB_FIRST)
        {
            fatal_error(0);  //only MSB first is supported!
        }

        // SPI configuration
        const uint8_t flags    = SPI_MODE_0;  ///< configuration flags
        const uint8_t wordSize = 8;           ///< Number of bits per transaction
        const uint32_t speed   = 1000000;     ///< device baud rate

        const sr_t ret = m_accessSpi->configure(i, flags, wordSize, speed);
        if (ret != E_SUCCESS)
        {
            fatal_error(FATAL_ERROR_SPI_CONFIG_FAILED);
        }
    }
}
