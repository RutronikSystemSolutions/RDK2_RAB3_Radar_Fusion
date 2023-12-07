/**
 * \addtogroup      II2c
 * \brief
 * This is a generic interface for an arbitrary I2C Master implementation
 * to access connected devices.
 * @{
 */
#ifndef II2C_H
#define II2C_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/i2c_definitions.h>

typedef struct
{
    /**
    * @return maximum accepted length of a buffer for a transmission,
    * which is specific to the implementation of the interface.
    */
    uint32_t (*getMaxTransfer)(void);

    /**
    * Change the speed of an I2C bus.
    * This is generic for the whole bus, so all devices connected to it must support it.
    *
    * @param devAddr specifies the bus to configure by the busId contained in devAddr
    * @param speed specifies the speed in Hz
    * @return Strata error code
    */
    sr_t (*configureBusSpeed)(uint16_t devAddr, uint32_t speed);

    /**
    * Clears an I2C bus.
    *
    * @param devAddr specifies the bus to reset by the busId contained in devAddr
    * @return Strata error code
    */
    sr_t (*clearBus)(uint16_t devAddr);

    /**
    * Waits for an ACK from an I2C device.
    * 
    * @param devAddr the device address (incl. busId)
    * @return Strata error code
    */
    sr_t (*pollForAck)(uint16_t devAddr);

    /**
     * Read register(s) from an I2C device.
     *
     * @param devAddr the device address (incl. busId)
     * @param length number of bytes to be read
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*readWithoutPrefix)(uint16_t devAddr, uint16_t length, uint8_t buffer[]);
    /**
     * Read register(s) from an I2C device 8-bit address.
     *
     * @param devAddr the device address (incl. busId)
     * @param prefix the 8-bit prefix to be written
     * @param length number of bytes to be read
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*readWith8BitPrefix)(uint16_t devAddr, uint8_t prefix, uint16_t length, uint8_t buffer[]);
    /**
     * Read register(s) from an I2C device 16-bit address.
     *
     * @param devAddr the device address (incl. busId)
     * @param prefix the 16-bit prefix to be written
     * @param length number of bytes to be read
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*readWith16BitPrefix)(uint16_t devAddr, uint16_t prefix, uint16_t length, uint8_t buffer[]);

    /**
     * Write register(s) to an I2C device.
     *
     * @param devAddr the device address (incl. busId)
     * @param length number of bytes to be written
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*writeWithoutPrefix)(uint16_t devAddr, uint16_t length, const uint8_t buffer[]);
    /**
     * Write register(s) to an I2C device 8-bit address.
     *
     * @param devAddr the device address (incl. busId)
     * @param prefix the 8-bit prefix to be read
     * @param length number of bytes to be written
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*writeWith8BitPrefix)(uint16_t devAddr, uint8_t prefix, uint16_t length, const uint8_t buffer[]);
    /**
     * Write register(s) to an I2C device 16-bit address.
     *
     * @param devAddr the device address (incl. busId)
     * @param prefix the 16-bit prefix to be read
     * @param length number of bytes to be written
     * @param buffer a buffer of the specified length
     * @return Strata error code
     */
    sr_t (*writeWith16BitPrefix)(uint16_t devAddr, uint16_t prefix, uint16_t length, const uint8_t buffer[]);
} II2c;

#endif /* II2C_H */

/** @} */
