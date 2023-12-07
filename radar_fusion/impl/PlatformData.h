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
 * \addtogroup        PlatformData
 * @{
 */
#ifndef PLATFORM_DATA_H
#define PLATFORM_DATA_H 1

#include <common/errors.h>
#include <impl/PlatformDataDefinition.h>
#include <universal/data_definitions.h>


sr_t PlatformData_initialize(const PlatformDataDefinition_t *definition, uint8_t count);


// todo: make platform specific only
sr_t PlatformData_calibrationRequired(uint8_t index, double dataRate, bool *isRequired);

/******************************************************************************/
/*Interface Methods Declaration ----------------------------------------------*/
/******************************************************************************/
sr_t PlatformData_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize);
sr_t PlatformData_calibrate(uint8_t index);
sr_t PlatformData_start(uint8_t index);
sr_t PlatformData_stop(uint8_t index);
sr_t PlatformData_getStatusFlags(uint8_t index, uint32_t *flags);
sr_t PlatformData_registerCallback(IData_callback callback, void *arg);

#endif /* PLATFORM_DATA_H */

/** @} */
