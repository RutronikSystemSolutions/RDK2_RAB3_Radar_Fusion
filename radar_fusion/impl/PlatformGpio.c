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

#include "PlatformGpio.h"

#include <stddef.h>

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/
#define PIO_MAX_IDX         (31)
#define IOPORT_PIN_INVALID  (0xFFFFFFFF)
#define IOPORT_MODE_DEFAULT (1 << 8) /*!< None of the other defines will be handled */

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Interface Methods Definition -----------------------------------------------*/
/******************************************************************************/

sr_t PlatformGpio_initialize(const PlatformGpioDefinition_t *definition, uint8_t count)
{
	return E_SUCCESS;
}

sr_t PlatformGpio_configurePin(uint16_t id, uint8_t flags)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformGpio_setPin(uint16_t id, bool state)
{
	return E_NOT_IMPLEMENTED;
}

sr_t PlatformGpio_getPin(uint16_t id, bool *state)
{
	return E_NOT_IMPLEMENTED;
}
