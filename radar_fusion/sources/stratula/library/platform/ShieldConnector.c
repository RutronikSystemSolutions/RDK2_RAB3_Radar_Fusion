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

#include "ShieldConnector.h"

#include <impl/PlatformGpio.h>
#include <impl/PlatformI2c.h>
#include <impl/thread.h>


sr_t ShieldConnector_detectV9(const ShieldConnectorDefinition_t *definition)
{
//    /* Detect the presence of a V9 shield.
//     *
//     * If a shield is connected correctly, its OD1 and OD2 lines will be pulled to a known
//     * state: OD1 to logic low and OD2 to logic high respectively.
//     */
//
//    bool od1, od2;
//    PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT_PULL_UP);
//    PlatformGpio_configurePin(definition->od2, GPIO_MODE_INPUT_PULL_UP);
//    this_thread_sleep_for(chrono_milliseconds(1));
//    PlatformGpio_getPin(definition->od1, &od1);
//    PlatformGpio_getPin(definition->od2, &od2);
//    PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT);
//    PlatformGpio_configurePin(definition->od2, GPIO_MODE_INPUT);
//    if (od1 == false && od2 == true)
//    {
//        // V9 shield detected
//        return E_SUCCESS;
//    }
//    // no shield attached
    return E_NOT_AVAILABLE;
}


sr_t ShieldConnector_detect(const ShieldConnectorDefinition_t *definition, uint8_t connectorId)
{
//    /* Detect the presence of a shield.
//     *
//     * If a shield is connected correctly, its I2C lines will be pulled to logic high
//     * by the bus pull-ups.
//     * If it is connected mirrored, its OD1/OD2 lines (which are positioned opposite
//     * of the I2C lines) will be pulled to logic high by the bus pull-ups.
//     *
//     * To put potentially connected I2C devices in a defined state:
//     * - An I2C clear bus command is issued on the I2C bus lines
//     * - An I2C clear bus command is emulated on the OD1/OD2 pins
//     */
//
//    uint8_t levels;
//
//    const sr_t i2cAvailable = PlatformI2c_clearBus(I2C_ADDR(connectorId, 0));
//    // configure i2c lines to GPIO mode in order to read their levels
//    if (i2cAvailable == E_SUCCESS)
//    {
//        RETURN_ON_ERROR(PlatformI2c_enableBus(I2C_ADDR(connectorId, 0), false));
//        // wait for line levels to stabilize before reading them
//        this_thread_sleep_for(chrono_milliseconds(1));
//        const sr_t retReadBus = PlatformI2c_readBus(I2C_ADDR(connectorId, 0), &levels);
//        // always restore I2C bus to default mode
//        const sr_t retEnableBus = PlatformI2c_enableBus(I2C_ADDR(connectorId, 0), true);
//
//        if (retEnableBus)
//        {
//            return retEnableBus;
//        }
//        if (retReadBus)
//        {
//            return retReadBus;
//        }
//    }
//    else
//    {
//        levels = 0;
//    }
//
//    /* To emulate an I2C clear bus command on the OD1/OD2 pins,
//     * the clock line is toggled 9 times using the GPIO interface.
//     */
//    PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT);
//    for (int i = 0; i < 9; i++)
//    {
//        PlatformGpio_configurePin(definition->od1, GPIO_MODE_OUTPUT_OPEN_DRAIN);
//        this_thread_sleep_for(chrono_milliseconds(1));
//        PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT);
//        this_thread_sleep_for(chrono_milliseconds(1));
//    }
//
//    // enable internal pull-downs for correct detection
//    PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT_PULL_DOWN);
//    PlatformGpio_configurePin(definition->od2, GPIO_MODE_INPUT_PULL_DOWN);
//
//    // wait for line levels to stabilize before reading them
//    this_thread_sleep_for(chrono_milliseconds(1));
//
//    bool state;
//    PlatformGpio_getPin(definition->od1, &state);
//    levels |= (state) ? 0x04 : 0;
//    PlatformGpio_getPin(definition->od2, &state);
//    levels |= (state) ? 0x08 : 0;
//
//    // restore lines to default high impedance state
//    PlatformGpio_configurePin(definition->od1, GPIO_MODE_INPUT);
//    PlatformGpio_configurePin(definition->od2, GPIO_MODE_INPUT);
//
//    /* If a shield is present, either the regular I2C lines (correct connection)
//     * or the opposite lines (mirrored connection) have been set to logic high.
//     */
//    switch (levels)
//    {
//        case 0x00:
//            // no shield attached (note: only accurate when i2c available)
//            if (i2cAvailable == E_SUCCESS)
//            {
//                return E_NOT_AVAILABLE;
//            }
//            break;
//        case 0x03:
//            // shield connected correctly
//            break;
//        case 0x0C:
//            // shield connected mirrored
//            return E_NOT_POSSIBLE;
//        default:
//            // invalid levels detected
//            return E_NOT_SUPPORTED;
//    }

    return E_SUCCESS;
}

sr_t ShieldConnector_enable(const ShieldConnectorDefinition_t *definition, bool enable)
{
//    if (enable)
//    {
//        /* enable SPI level shifter */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_spi_oe, GPIO_MODE_OUTPUT_PUSH_PULL));
//
//        /* set direction of GPIO level shifter and enable */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_gpio_dir, GPIO_MODE_OUTPUT_PUSH_PULL));
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_gpio_oe, GPIO_MODE_OUTPUT_PUSH_PULL));
//
//        /* power-up shield by enabling its LDO */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->en_ldo, GPIO_MODE_OUTPUT_PUSH_PULL | GPIO_FLAG_OUTPUT_INITIAL_HIGH));
//        this_thread_sleep_for(chrono_milliseconds(10));
//    }
//    else
//    {
//        /* disable SPI level shifter */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_spi_oe, GPIO_MODE_OUTPUT_PUSH_PULL | GPIO_FLAG_OUTPUT_INITIAL_HIGH));
//
//        /* return GPIO level shifter pins to default high impedance state */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_gpio_oe, GPIO_MODE_INPUT));
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->ls_gpio_dir, GPIO_MODE_INPUT));
//
//        /* power-down shield by switching off its LDO */
//        RETURN_ON_ERROR(PlatformGpio_configurePin(definition->en_ldo, GPIO_MODE_INPUT));
//    }

    return E_SUCCESS;
}
