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

#include "PlatformInterrupt.h"

#include "ConfigurationIsr.h"
#include <common/typeutils.h>

#include "radar_irq.h"
#include "IPinsAvian.h"

/******************************************************************************/
/*Struct Definitions ----------------------------------------------------------*/
/******************************************************************************/
typedef struct
{
    /** \brief callback function for platform interrupt handler */
    void (*p_handler)(uint32_t, uint32_t);
    /** \brief callback function to invoke from platform interrupt handler */
    void (*fn)(void *);
    /** \brief context to pass to the callback function */
    void *arg;
} InterruptHandler_t;

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Public Methods Definition --------------------------------------------------*/
/******************************************************************************/

sr_t PlatformInterrupt_registerCallback(const PlatformInterruptDefinition_t *definition, void(fn)(void *), void *arg)
{
	func = fn;
	rad_arg = arg;
	return E_SUCCESS;
}

sr_t PlatformInterrupt_enable(const PlatformInterruptDefinition_t *definition, bool enable)
{
	IPinsAvianConfig_t *avian_pins = (IPinsAvianConfig_t *)definition;

	if(avian_pins->gpioIrq == 17158) /*TODO: implement interrupt recognition*/
	{
		cyhal_gpio_enable_event(RAD_IRQ, CYHAL_GPIO_IRQ_RISE, RAD_IRQ_PRIORITY, enable);
		return E_SUCCESS;
	}
	return E_FAILED;
}
