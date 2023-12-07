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

#ifndef PLATFORM_INTERRUPT_H_
#define PLATFORM_INTERRUPT_H_

#include <common/errors.h>
#include <impl/PlatformInterruptDefinition.h>
#include <stdbool.h>
#include <stdint.h>


/** \brief Configure an IRQ pin and register the interrupt handler callback function
 *         Note: the interrupt handler needs to be enabled via PlatformInterrupt_enable()
 *
 * \param definition platform-specific interrupt pin definition
 * \param fn         handler callback function to invoke from the IRQ pin interrupt handler
 * \param arg        context to pass to callback function
 */
sr_t PlatformInterrupt_registerCallback(const PlatformInterruptDefinition_t *definition, void (*fn)(void *), void *arg);

/** \brief Enable/disable specific interrupt handler
 *
 * \param definition platform-specific interrupt pin definition
 * \param enable     true if enable, otherwise disable
 */
sr_t PlatformInterrupt_enable(const PlatformInterruptDefinition_t *definition, bool enable);


#endif /* PLATFORM_INTERRUPT_H_ */