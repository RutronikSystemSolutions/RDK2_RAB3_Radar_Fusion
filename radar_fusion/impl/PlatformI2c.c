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

#include "PlatformI2c.h"
#include <impl/chrono.h>
#include <impl/thread.h>

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/
// Maximum number of bytes per i2c-transfer
#define PLATFORM_I2C_MAX_TRANSFER_SIZE UINT16_MAX
// i2c Ack Polling Timeout(ms)
#define PLATFORM_I2C_TRANSFER_TIMEOUT 100
/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Local Function Prototypes  -------------------------------------------------*/
/******************************************************************************/

/**
 * Translates Atmel Error into Strata Error Definition.
 *
 * @param	status  uint32_t with Atmel definition
 * @return	Strata error code
 */
//static inline sr_t translateError(uint32_t status);

/**
 * Configures i2c bus sda and scl pins in GPIO mode or peripheral mode.
 *
 * @param bus		PlatformI2cDefinition_t i2c bus pointer
 * @param enable	bool					true -> configures pin as Peripheral, false -> configure pins as GPIO (disables I2c function)
 * @return none
 */
//static void configureBusPins(const PlatformI2cDefinition_t *bus, bool enable);

/**
 * write implementation for i2c.
 *
 * @param  devAddr	uint16_t contains device address
 * @param  length	uint16_t packet length
 * @param  packet   twihs_packet_t* pointer to atmel packet structure
 * @param  buffer   uint8_t[] pointer to write buffer
 * @return Strata error code
 */
//static sr_t writeImpl(uint16_t devAddr, uint16_t length, twihs_packet_t *packet, const uint8_t buffer[]);

/**
 * read implementation for i2c.
 *
 * @param  devAddr	uint16_t contains device address
 * @param  length	uint16_t packet length
 * @param  packet   twihs_packet_t* pointer to atmel packet structure
 * @param  buffer   uint8_t[] pointer to read buffer
 * @return Strata error code
 */
//static sr_t readImpl(uint16_t devAddr, uint16_t length, twihs_packet_t *packet, uint8_t buffer[]);

/******************************************************************************/
/*Local Function Definitions -------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Interface Methods Definition -----------------------------------------------*/
/******************************************************************************/

sr_t PlatformI2c_writeWithoutPrefix(uint16_t devAddr, uint16_t length, const uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_writeWith8BitPrefix(uint16_t devAddr, uint8_t prefix, uint16_t length, const uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_writeWith16BitPrefix(uint16_t devAddr, uint16_t prefix, uint16_t length, const uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_readWithoutPrefix(uint16_t devAddr, uint16_t length, uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_readWith8BitPrefix(uint16_t devAddr, uint8_t prefix, uint16_t length, uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_readWith16BitPrefix(uint16_t devAddr, uint16_t prefix, uint16_t length, uint8_t buffer[])
{
	return E_NOT_IMPLEMENTED;
}

uint32_t PlatformI2c_getMaxTransfer(void)
{
    return PLATFORM_I2C_MAX_TRANSFER_SIZE;
}

sr_t PlatformI2c_configureBusSpeed(uint16_t devAddr, uint32_t speed)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_pollForAck(uint16_t devAddr)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_clearBus(uint16_t devAddr)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_enableBus(uint16_t devAddr, bool enable)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_readBus(uint16_t devAddr, uint8_t *levels)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformI2c_initialize(const PlatformI2cDefinition_t *definition, uint8_t busCount)
{
	return E_SUCCESS;
}
