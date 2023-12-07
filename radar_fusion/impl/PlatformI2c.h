/* ===========================================================================
** Copyright (C) 2021 Infineon Technologies AG
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
** ===========================================================================
*/

/**
 * \addtogroup      I2cPlatform
 * @{
 */
#ifndef PLATFORM_I2C_H
#define PLATFORM_I2C_H 1

#include "PlatformI2cDefinition.h"
#include <common/errors.h>
#include <universal/i2c_definitions.h>
#include <stdbool.h>

sr_t PlatformI2c_initialize(const PlatformI2cDefinition_t *buses, uint8_t busCount);

/**
* Set I2C bus to enabled or disabled
*
* @param devAddr specifies the bus to enable by the busId contained in devAddr
* @param enable configures bus in peripheral mode when true, configures bus in GPIO mode when false
* @return Strata error code
*/
sr_t PlatformI2c_enableBus(uint16_t devAddr, bool enable);

/**
* Read the levels of an I2C bus.
*
* @param devAddr specifies the bus to configure by the busId contained in devAddr
* @param levels  pointer to read levels. this memory is first set to 0 before populating the level readings.
* @return Strata error code
*/
sr_t PlatformI2c_readBus(uint16_t devAddr, uint8_t *levels);

/**
* Waits for an ACK from an I2C device.
* 
* @param devAddr the device address to poll
* @return Strata error code
*/
sr_t PlatformI2c_pollForAck(uint16_t devAddr);

uint32_t PlatformI2c_getMaxTransfer(void);
sr_t PlatformI2c_writeWithoutPrefix(uint16_t devAddr, uint16_t length, const uint8_t buffer[]);
sr_t PlatformI2c_writeWith8BitPrefix(uint16_t devAddr, uint8_t prefix, uint16_t length, const uint8_t buffer[]);
sr_t PlatformI2c_writeWith16BitPrefix(uint16_t devAddr, uint16_t prefix, uint16_t length, const uint8_t buffer[]);
sr_t PlatformI2c_readWithoutPrefix(uint16_t devAddr, uint16_t length, uint8_t buffer[]);
sr_t PlatformI2c_readWith8BitPrefix(uint16_t devAddr, uint8_t prefix, uint16_t length, uint8_t buffer[]);
sr_t PlatformI2c_readWith16BitPrefix(uint16_t devAddr, uint16_t prefix, uint16_t length, uint8_t buffer[]);

sr_t PlatformI2c_configureBusSpeed(uint16_t devAddr, uint32_t speed);
sr_t PlatformI2c_clearBus(uint16_t devAddr);


#endif /* PLATFORM_I2C_H */

/** @} */
