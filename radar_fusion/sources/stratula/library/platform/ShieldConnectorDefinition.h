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

#ifndef SHIELD_CONNECTOR_DEFINITION_H
#define SHIELD_CONNECTOR_DEFINITION_H 1

#include <stdint.h>
#include <universal/gpio_definitions.h>


/*
 * irq0, irq1 can be used as GPO by changing direction of level-shifter
 * csn0, csn1 can be used as GPI by changing direction of level-shifter
 */

typedef struct
{
    uint16_t en_ldo;       ///< enable for LDO (onboard 5V to 3.3V converter) - onboard
    uint16_t ls_spi_oe;    ///< enable for SPI level shifters - onboard
    uint16_t ls_gpio_oe;   ///< enable for GPIO level shifter - onboard
    uint16_t ls_gpio_dir;  ///< direction of GPIO level shifter - onboard
    uint16_t od_led;       ///< open-drain LED signal (GPIO)
    uint16_t od1;          ///< open-drain 1 (used for shield detection)
    uint16_t od2;          ///< open-drain 2 (used for shield detection)
    uint16_t od3;          ///< open-drain 3 (GPIO)
    uint16_t od4;          ///< open-drain 4 (GPIO)
    uint16_t irq0;         ///< IRQ0 (GPI, or used for IRQ) (BGT_IRQ)
    uint16_t irq1;         ///< IRQ1 (GPI, or used for IRQ) (Legacy: GPIO2, Plus: BGT_IRQ2)
    uint16_t dio2;         ///< DIO2 (GPO, or used for QSPI) (Legacy: GPIO1, coupled as GPI with GPIO2!)
    uint16_t dio3;         ///< DIO3 (GPO, or used for QSPI) (Legacy: BGT_RST)
    uint16_t csn0;         ///< CSN0 (GPO, or used for (Q)SPI) (Plus V9: SPI_CSN_V9)
    uint16_t csn1;         ///< CSN1 (GPO, or used for (Q)SPI) (Plus V9: not connected)
} ShieldConnectorDefinition_t;


#endif /* SHIELD_CONNECTOR_DEFINITION_H */
