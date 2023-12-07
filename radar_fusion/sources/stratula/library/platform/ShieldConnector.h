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

#ifndef SHIELD_CONNECTOR_H_
#define SHIELD_CONNECTOR_H_

#include "ShieldConnectorDefinition.h"

#include <common/errors.h>


/** \brief Detects the presence of a V9 shield by probing certain pins.
 * 
 *         Returns an error code according to detection status (attached / not attached).
 *
 * \param definition definition struct of a particular shield connector
 */
sr_t ShieldConnector_detectV9(const ShieldConnectorDefinition_t *definition);

/** \brief Detects the presence of a shield by probing the i2c lines.
 * 
 *         Returns an error code according to detection status (attached / not attached / mirrored / invalid).
 *
 * \param definition definition struct of a particular shield connector
 * \param connectorId connector index
 */
sr_t ShieldConnector_detect(const ShieldConnectorDefinition_t *definition, uint8_t connectorId);

/** \brief Enable/Disable connector access peripherals.
 *         This function turns on the LDO which supplies power to the connected shield,
 *         and configures the level shifters used for SPI and GPIO access.
 *
 *         Returns an error code in case access has failed.
 * 
 * \param definition definition struct of a particular shield connector
 * \param connectorId connector index
 * \param enable enable/disable switch
 */
sr_t ShieldConnector_enable(const ShieldConnectorDefinition_t *definition, bool enable);


#endif /* SHIELD_CONNECTOR_H_ */
