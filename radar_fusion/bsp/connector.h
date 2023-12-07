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

#include <platform/ShieldConnectorDefinition.h>


static const uint16_t BOARD_DETECTION_PIN = GPIO_ID('A', 8);
static const uint16_t SPI_CSN_V9          = GPIO_ID('A', 7);


/* Hatvan Legacy Board Connector Definition */
static ShieldConnectorDefinition_t ShieldConnectorDefinitionHatvanLegacy[] = {
    [0] = {
        .en_ldo      = GPIO_ID('C', 30),  ///< enable for LDO (onboard 5V to 3.3V converter) - onboard
        .ls_spi_oe   = GPIO_ID('D', 24),  ///< enable for SPI level shifters - onboard
        .ls_gpio_oe  = GPIO_ID('D', 14),  ///< enable for GPIO level shifter - onboard
        .ls_gpio_dir = GPIO_ID('D', 18),  ///< direction of GPIO level shifter - onboard
        .od_led      = GPIO_ID('C', 1),   ///< open-drain LED signal (GPIO)
        .od1         = GPIO_ID('D', 16),  ///< open-drain 1 (used for shield detection)
        .od2         = GPIO_ID('C', 9),   ///< open-drain 2 (used for shield detection)
        .od3         = GPIO_ID('C', 28),  ///< open-drain 3 (GPIO)
        .od4         = GPIO_ID('A', 27),  ///< open-drain 4 (GPIO)
        .irq0        = GPIO_ID('C', 6),   ///< IRQ0 (GPI, or used for IRQ)
        .irq1        = GPIO_ID('A', 10),  ///< IRQ1 (GPI, or used for IRQ) (Legacy: GPIO2)
        .dio2        = GPIO_ID('A', 9),   ///< DIO2 (GPO, or used for QSPI) (Legacy: GPIO1, coupled as GPI with GPIO2!)
        .dio3        = GPIO_ID('A', 0),   ///< DIO3 (GPO, or used for QSPI) (Legacy: BGT_RST)
        .csn0        = GPIO_ID('A', 11),  ///< CSN0 (GPO, or used for (Q)SPI) (Plus V9: SPI_CSN_V9)
        .csn1        = GPIO_NAME_NONE,    ///< CSN1 (GPO, or used for (Q)SPI)
    },
    [1] = {
        .en_ldo      = GPIO_ID('C', 31),  ///< enable for LDO (onboard 5V to 3.3V converter) - onboard
        .ls_spi_oe   = GPIO_ID('C', 3),   ///< enable for SPI level shifters - onboard
        .ls_gpio_oe  = GPIO_ID('A', 1),   ///< enable for GPIO level shifter - onboard
        .ls_gpio_dir = GPIO_ID('C', 10),  ///< direction of GPIO level shifter - onboard
        .od_led      = GPIO_ID('C', 2),   ///< open-drain LED signal (GPIO)
        .od1         = GPIO_ID('A', 22),  ///< open-drain 1 (used for shield detection)
        .od2         = GPIO_ID('D', 10),  ///< open-drain 2 (used for shield detection)
        .od3         = GPIO_ID('C', 29),  ///< open-drain 3 (GPIO)
        .od4         = GPIO_ID('D', 15),  ///< open-drain 4 (GPIO)
        .irq0        = GPIO_ID('C', 11),  ///< IRQ0 (GPI, or used for IRQ)
        .irq1        = GPIO_ID('A', 2),   ///< IRQ1 (GPI, or used for IRQ) (Legacy: GPIO2)
        .dio2        = GPIO_ID('A', 7),   ///< DIO2 (GPO, or used for QSPI) (Legacy: GPIO1, coupled as GPI with GPIO2!)
        .dio3        = GPIO_ID('A', 17),  ///< DIO3 (GPO, or used for QSPI) (Legacy: BGT_RST)
        .csn0        = GPIO_ID('C', 25),  ///< CSN0 (GPO, or used for (Q)SPI) (Plus V9: SPI_CSN_V9)
        .csn1        = GPIO_NAME_NONE,    ///< CSN1 (GPO, or used for (Q)SPI)
    },
};

/* Hatvan Plus Board Connector Definition
   Note: HatvanPlus v3.1 schematic labels are wrong at "Sensor Connector 2 (SC2)"
         OD1 & OCD are swapped with OD3 & OD4
 */
//static ShieldConnectorDefinition_t ShieldConnectorDefinitionHatvanPlus[] = {
//    [0] = {
//        .en_ldo      = GPIO_ID('C', 30),  ///< enable for LDO (onboard 5V to 3.3V converter) - onboard
//        .ls_spi_oe   = GPIO_ID('D', 24),  ///< enable for SPI level shifters - onboard
//        .ls_gpio_oe  = GPIO_NAME_NONE,    ///< enable for GPIO level shifter - onboard
//        .ls_gpio_dir = GPIO_NAME_NONE,    ///< direction of GPIO level shifter - onboard
//        .od_led      = GPIO_ID('C', 1),   ///< open-drain LED signal (GPIO)
//        .od1         = GPIO_ID('D', 16),  ///< open-drain 1 (used for shield detection)
//        .od2         = GPIO_ID('C', 9),   ///< open-drain 2 (used for shield detection)
//        .od3         = GPIO_ID('C', 28),  ///< open-drain 3 (GPIO)
//        .od4         = GPIO_ID('A', 27),  ///< open-drain 4 (GPIO)
//        .irq0        = GPIO_ID('C', 6),   ///< IRQ0 (GPI, or used for IRQ)
//        .irq1        = GPIO_ID('A', 25),  ///< IRQ1 (GPI, or used for IRQ) (Legacy: GPIO2)
//        .dio2        = GPIO_ID('A', 17),  ///< DIO2 (GPO, or used for QSPI) (Legacy: GPIO1, coupled as GPI with GPIO2!)
//        .dio3        = GPIO_ID('D', 31),  ///< DIO3 (GPO, or used for QSPI) (Legacy: BGT_RST)
//        .csn0        = GPIO_ID('A', 11),  ///< CSN0 (GPO, or used for (Q)SPI) (Plus V9: SPI_CSN_V9)
//        .csn1        = GPIO_ID('A', 9),   ///< CSN1 (GPO, or used for (Q)SPI)
//    },
//};
