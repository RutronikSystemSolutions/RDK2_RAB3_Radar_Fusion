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
/*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*/

#include "PlatformSpi.h"
#include "spi_interface.h"

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/
#ifndef PLATFORM_SPI_MAX_DEVICES
#define PLATFORM_SPI_MAX_DEVICES 1u
#endif

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/* Function Prototypes ------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/* Function Definitions ------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Interface Methods Definition -----------------------------------------------*/
/******************************************************************************/

sr_t PlatformSpi_readBurstAsync8(uint8_t devId, uint8_t *bufRead, uint32_t count, void (*cb)(void *), void *arg)
{

    if(spi_irq_data.spi_lock)
    {
    	return E_BUSY;
    }
    spi_irq_data.spi_lock = true;
    spi_irq_data.async = true;
    spi_irq_data.callback = cb;
    spi_irq_data.arg = arg;
    spi_irq_data.slice = count/BURST_SLICE_SIZE - 1;

    if(count%BURST_SLICE_SIZE != 0)
    {
    	if(count < BURST_SLICE_SIZE)
    	{
    		spi_irq_data.slice = 0;
    		spi_irq_data.split = count;
    	    spi_irq_data.read_buff = bufRead;
    	    cyhal_spi_transfer_async(&spi_radar_obj, NULL, 0, bufRead, spi_irq_data.split);
    	    return E_SUCCESS;
    	}
    	else
    	{
    		spi_irq_data.split = count - count/BURST_SLICE_SIZE;
    	}
    }
    spi_irq_data.read_buff = bufRead;
    cyhal_spi_transfer_async(&spi_radar_obj, NULL, 0, bufRead, BURST_SLICE_SIZE);

	return E_SUCCESS;
}

sr_t PlatformSpi_readBurstAsync12(uint8_t devId, uint16_t *bufRead, uint32_t count, void (*cb)(void *), void *arg)
{
	return E_NOT_IMPLEMENTED;
}

uint32_t PlatformSpi_getMaxTransfer(void)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_configure(uint8_t devId, uint8_t flags, uint8_t wordSize, uint32_t speed)
{
    return E_SUCCESS;
}

sr_t PlatformSpi_write8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_write16(uint8_t devId, uint32_t count, const uint16_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_write32(uint8_t devId, uint32_t count, const uint32_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_read8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_read16(uint8_t devId, uint32_t count, uint16_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_read32(uint8_t devId, uint32_t count, uint32_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_writeQspi8(uint8_t devId, uint32_t count, const uint8_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_readQspi8(uint8_t devId, uint32_t count, uint8_t buffer[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_transfer8(uint8_t devId, uint32_t count, const uint8_t bufOut[], uint8_t bufIn[], bool keepSel)
{
	cy_rslt_t result;

    if (count == 0)
    {
        return E_SUCCESS;
    }

    if(spi_irq_data.spi_lock)
    {
    	return E_BUSY;
    }

    spi_cs_enable();

    spi_irq_data.spi_lock = true;
    result = cyhal_spi_transfer(&spi_radar_obj, bufOut, count, bufIn, count, 0xFF);
    spi_irq_data.spi_lock = false;

    if (!keepSel)
    {
    	spi_cs_disable();
    }

    if(result != CY_RSLT_SUCCESS)
    {
    	return E_FAILED;
    }

    return E_SUCCESS;
}

sr_t PlatformSpi_transfer16(uint8_t devId, uint32_t count, const uint16_t bufOut[], uint16_t bufIn[], bool keepSel)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_transfer32(uint8_t devId, uint32_t count, const uint32_t bufOut[], uint32_t bufIn[], bool keepSel)
{
    return E_NOT_IMPLEMENTED;
}

sr_t PlatformSpi_initialize(PlatformSpiDefinition_t *definition, uint8_t count)
{
    return E_SUCCESS;
}
