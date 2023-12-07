/**
 * \addtogroup      ISpi
 * \brief
 * This is a generic interface for supporting arbitrary Serial Peripheral Interface
 * access to connected devices. Each instance of this class represents one SPI master.
 * @{
 */
#ifndef ISPI_H
#define ISPI_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/spi_definitions.h>


typedef struct
{
    /**
    * Returns the maximum accepted length of a buffer for a transmission,
    * which is specific to the implementation of the interface.
    *
    * @return length maximum accepted length, in bytes
    */
    uint32_t (*getMaxTransfer)(void);

    /**
    * Configure the clock speed for SPI transactions.
    * Optionally, specify a slave for which this setting is intended.
    *
    * @param devId the device ID for which this setting is intended
    * @param flags specifies the configuration flags
    * @param wordSize specifies the word length in bits
    * @param speed specifies the clock speed in Hz
    * @return Strata error code
    */
    sr_t (*configure)(uint8_t devId, uint8_t flags, uint8_t wordSize, uint32_t speed);

    /**
    * Reading data from an SPI device in half-duplex mode.
    *
    * @param devId the device ID that identifies the slave select signal to be used
    * @param count number of words to be read
    * @param buffer a buffer of the specified length
    * @param keepSel leave the slave-select line activated for another immediately following transaction
    * @return Strata error code
    */
    sr_t (*read8)(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel);
    sr_t (*read16)(uint8_t devId, uint32_t count, uint16_t buffer[], bool keepSel);
    sr_t (*read32)(uint8_t devId, uint32_t count, uint32_t buffer[], bool keepSel);

    /**
    * Write data to an SPI device in half-duplex mode.
    *
    * @param devId the device ID that identifies the slave select signal to be used
    * @param count number of words to be written
    * @param buffer a buffer of the specified length
    * @param keepSel leave the slave-select line activated for another immediately following transaction
    * @return Strata error code
    */
    sr_t (*write8)(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel);
    sr_t (*write16)(uint8_t devId, uint32_t count, const uint16_t buffer[], bool keepSel);
    sr_t (*write32)(uint8_t devId, uint32_t count, const uint32_t buffer[], bool keepSel);

    /**
    * Writing/Reading data from an SPI device in full-duplex mode.
    *
    * @param devId the device ID that identifies the slave select signal to be used
    * @param count number of words to be read
    * @param bufWrite buffer of the specified length to be written
    * @param bufRead buffer of the specified length to be read into
    * @param keepSel leave the slave-select line activated for another immediately following transaction
    * @return Strata error code
    */
    sr_t (*transfer8)(uint8_t devId, uint32_t count, const uint8_t bufWrite[], uint8_t bufRead[], bool keepSel);
    sr_t (*transfer16)(uint8_t devId, uint32_t count, const uint16_t bufWrite[], uint16_t bufRead[], bool keepSel);
    sr_t (*transfer32)(uint8_t devId, uint32_t count, const uint32_t bufWrite[], uint32_t bufRead[], bool keepSel);
} ISpi;

#endif /* ISPI_H */

/** @} */
