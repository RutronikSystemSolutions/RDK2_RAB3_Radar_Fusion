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
 * \addtogroup      SpiPlatform
 * @{
 */
#ifndef PLATFORM_SPI_H
#define PLATFORM_SPI_H 1

#include <common/errors.h>
#include <impl/PlatformSpiDefinition.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/spi_definitions.h>


uint32_t PlatformSpi_getMaxTransfer(void);
sr_t PlatformSpi_configure(uint8_t devId, uint8_t flags, uint8_t wordSize, uint32_t speed);
sr_t PlatformSpi_write8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel);
sr_t PlatformSpi_write16(uint8_t devId, uint32_t count, const uint16_t buffer[], bool keepSel);
sr_t PlatformSpi_write32(uint8_t devId, uint32_t count, const uint32_t buffer[], bool keepSel);
sr_t PlatformSpi_read8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel);
sr_t PlatformSpi_read16(uint8_t devId, uint32_t count, uint16_t buffer[], bool keepSel);
sr_t PlatformSpi_read32(uint8_t devId, uint32_t count, uint32_t buffer[], bool keepSel);
sr_t PlatformSpi_transfer8(uint8_t devId, uint32_t count, const uint8_t bufOut[], uint8_t bufIn[], bool keepSel);
sr_t PlatformSpi_transfer16(uint8_t devId, uint32_t count, const uint16_t bufOut[], uint16_t bufIn[], bool keepSel);
sr_t PlatformSpi_transfer32(uint8_t devId, uint32_t count, const uint32_t bufOut[], uint32_t bufIn[], bool keepSel);

sr_t PlatformSpi_writeQspi8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel);
sr_t PlatformSpi_readQspi8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel);

sr_t PlatformSpi_initialize(PlatformSpiDefinition_t *definition, uint8_t count);

/**
 * \brief This function asynchronously reads captured 12bit sample data from the BGT60TRxx chip into 16bit buffer.
 *
 * This function reads the sample data from the memory integrated in the
 * BGT60TRxx chip with a SPI burst transfer.

 * This function allows to specify a callback function and associated data which will be called once the
 * transfer is completed.
 *
 * The whole burst transfer is performed in a single SPI frame, so the chip
 * select signal (CS) goes LOW at the beginning and stays LOW until the end of
 * the burst transfer.
 *
 * \param         devId             SPI device Id
 * \param[out]    bufRead           A pointer to a buffer where received data
 * \param[in]     count             The number of words to be transferred.
 * \param[in]     cb                Function to call once the transfer
 *                                  has finished.
 * \param[in]     arg               Pointer to pass to the callback
 *                                        function.
 *
 */
sr_t PlatformSpi_readBurstAsync12(uint8_t devId, uint16_t *bufRead, uint32_t count, void (*cb)(void *), void *arg);

/**
 * \brief This function asynchronously reads captured sample data from the BGT60TRxx chip. The transactions are performed
 *        byte-wise resulting in packed data. 
 *
 * This function reads the sample data from the memory integrated in the
 * BGT60TRxx chip with a SPI burst transfer.

 * This function allows to specify a callback function and associated data which will be called once the
 * transfer is completed.
 *
 * The whole burst transfer is performed in a single SPI frame, so the chip
 * select signal (CS) goes LOW at the beginning and stays LOW until the end of
 * the burst transfer.
 *
 * \param         devId             SPI device Id
 * \param[out]    bufRead           A pointer to a buffer where received data
 * \param[in]     count             The number of words to be transferred.
 * \param[in]     cb                Function to call once the transfer
 *                                  has finished.
 * \param[in]     arg               Pointer to pass to the callback
 *                                        function.
 *
 */
sr_t PlatformSpi_readBurstAsync8(uint8_t devId, uint8_t *bufRead, uint32_t count, void (*cb)(void *), void *arg);
#endif /* PLATFORM_SPI_H */

/** @} */
