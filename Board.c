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

#include <PlatformInterfaces.h>
#include <board/Board.h>
#include <bsp/avian.h>
#include <bsp/connector.h>
#include <bsp/leds.h>
#include <bsp/spi_custom.h>
#include <board/BoardInfo.h>
#include <common/typeutils.h>
#include <components/radar/Avian.h>
#include <impl/Platform.h>
#include <impl/PlatformGpio.h>
#include <impl/PlatformInterrupt.h>
#include <impl/PlatformSpi.h>
#include <impl/SamsHelper.h>
#include <impl/thread.h>
#include <platform/DataAvian.h>
#include <platform/ShieldConnector.h>
#include <platform/led/LedSequenceRbb.h>
#include <platform/led/LedSequenceStatus.h>
#include <protocol/CommandHandlerRadar.h>
#include <protocol/ProtocolHandler.h>
#include <protocol/RequestHandler.h>
#include <protocol/commands/Commands_IRadar.h>
#include "watchdog.h"

#include <string.h>  // for memcpy

// this includes the implementation of getMac() and getUuid() for the Board
#include <impl/getIdsAtmel.h>

#include <fatal_error.h>

#define DATA_BUFFER_SIZE       (192 * 1024)

/****************************************************************************
 * Variable declarations
 ****************************************************************************/
static Avian m_avian;
static IData *m_data = NULL;

// specify alignment to allow this buffer to be cast to uint16_t for different implementations
static uint8_t m_dataBuffer[DATA_BUFFER_SIZE] __attribute__((aligned(sizeof(uint16_t))));

extern PlatformSpiDefinition_t *BoardSpiDefinition;
static ShieldConnectorDefinition_t *ShieldConnectorDefinition;
static uint8_t m_shieldConnectorCount;

/****************************************************************************
 * Private methods
 ****************************************************************************/
static void Board_acquisitionStatusCallback(bool state)
{
    if (state)
    {
        LedSequence_setStatus(LED_STATUS_MEASURING);
    }
    else
    {
        LedSequence_setStatus(LED_STATUS_OPERATING);
    }
}

static void Board_dataCallback(void *arg, uint8_t *payload, uint32_t count, uint8_t channel, uint64_t timestamp)
{
    /* Sends data samples to upper layers
     */

    LedSequence_setStatus(LED_STATUS_TRANSFERRING);

    const sr_t ret = ProtocolHandler_sendDataFrame(payload, count, channel, timestamp);
    if (ret != E_SUCCESS)
    {
        m_data->stop(channel);
    }

    LedSequence_setStatus(LED_STATUS_OPERATING);
}

static sr_t Board_detectShields(void)
{
    /* Check all legacy connectors for unsupported or wrongly connected shields.
     * In case no errors are detected, all connectors will be enabled,
     * in order to proceed with radar device detection.
     */
    for (uint8_t shieldId = 0; shieldId < m_shieldConnectorCount; shieldId++)
    {
        // detect the presence of a shield by probing the I2C lines
        const sr_t detection = ShieldConnector_detect(&ShieldConnectorDefinition[shieldId], shieldId);
        switch (detection)
        {
            case E_NOT_AVAILABLE:  // allow shields without I2C bus
                /*  no break */
            case E_SUCCESS:
                break;
            case E_NOT_POSSIBLE:
                LedSequence_setRbbStatus(RBB_ERROR_HARDWARE_CONNECTED_WRONG);
                return E_NOT_POSSIBLE;
                break;
            case E_NOT_SUPPORTED:
                LedSequence_setRbbStatus(RBB_ERROR_HARDWARE_NOT_SUPPORTED);
                return E_NOT_SUPPORTED;
                break;
            default:
                LedSequence_setRbbStatus(RBB_ERROR_HARDWARE_INTERNAL_ERROR);
                return detection;
                break;
        }

        // power-up shield and configure its level shifters
        ShieldConnector_enable(&ShieldConnectorDefinition[shieldId], true);
    }

    return E_SUCCESS;
}

static IRadar *Board_detectRadar(IGpio *gpio, ISpi *spi, II2c *i2c, IData **data, bool hatvanPlus, bool useQspi)
{
    /* Checks both connectors for the presence of a supported radar device.
     * Only the first detected device will be instantiated.
     */
    for (uint8_t shieldId = 0; shieldId < m_shieldConnectorCount; shieldId++)
    {
        BoardRadarPinsConfigAvian->gpioReset = ShieldConnectorDefinition[shieldId].dio3;
        BoardRadarPinsConfigAvian->gpioIrq   = ShieldConnectorDefinition[shieldId].irq0;
        const sr_t avianDetection            = Avian_Detect(spi, gpio, BoardRadarConfigAvian, BoardRadarPinsConfigAvian);
        if (avianDetection == E_SUCCESS)
        {
            Avian_Constructor(&m_avian, &DataAvian, gpio, spi, BoardRadarConfigAvian, BoardRadarPinsConfigAvian);
            DataAvian_Constructor(Board_acquisitionStatusCallback);
            DataAvian_setBuffer(BoardRadarConfigAvian->dataIndex, m_dataBuffer, DATA_BUFFER_SIZE);
            //BoardIrqPinsConfigAvian->pin = PlatformGpio_getPortPin(ShieldConnectorDefinition[shieldId].irq0);
            DataAvian_initialize(BoardRadarConfigAvian->dataIndex, BoardIrqPinsConfigAvian, useQspi);
            *data = &DataAvian;
            return (IRadar *)&m_avian;
        }
    }

    LedSequence_setRbbStatus(RBB_ERROR_HARDWARE_NOT_DETECTED);
    return NULL;
}

/****************************************************************************
 * Public methods implementation
 ****************************************************************************/
void Board_Constructor(void)
{
    IGpio *gpio = &PlatformGpio;
    ISpi *spi   = &PlatformSpi;
    II2c *i2c   = &PlatformI2c;

    /*Initialize RDK2 Platform*/
    Platform_Constructor();

    // initialize LedSequence to be used for potential error signaling during detection
    LedSequence_Constructor();
    LedSequence_setStatus(LED_STATUS_OPERATING);
    
    /* The shield detection is different for different board types */
    sr_t shieldDetected;

    if (Sams70RevisionA())
    {
        // update character from 'B' to 'A' making name suffix " (MCU A)"
        boardInfo.name[sizeof(BOARD_NAME_HATVAN_LEGACY) - 3]--;
    }
    // select connector configuration
    BoardSpiDefinition        = BoardSpiDefinitionHatvanLegacy;
    ShieldConnectorDefinition = ShieldConnectorDefinitionHatvanLegacy;
    m_shieldConnectorCount    = ARRAY_SIZE(ShieldConnectorDefinitionHatvanLegacy);
    shieldDetected            = E_NOT_AVAILABLE;  // V9 connector not present
    PlatformSpi_initialize(BoardSpiDefinition, m_shieldConnectorCount);

    /*
    1) check for shield connector error
    mirrored connection can only be detected if the shield has I2C pull-ups
    other unexpected levels on OC pins are interpreted as unsupported shield connection
    */

    IRadar *radar = NULL;

    if (shieldDetected != E_SUCCESS)
    {
        shieldDetected = Board_detectShields();
    }

    if (shieldDetected == E_SUCCESS)
    {
        /*
        2) Check presence of known devices using SPI and I2C
        */
    	radar = Board_detectRadar(gpio, spi, i2c, &m_data, false, false);
    }

    /*
    3) Continue with normal initialization, so that even without a detected device, I2C, SPI, GPIO, etc. are usable
    */

    // Enable communication interface
    RequestHandler_Constructor(gpio, spi, m_data, i2c);
    ProtocolHandler_Constructor();
    CommandHandlerRadar_Constructor();
    if (radar != NULL)
    {
        Commands_IRadar_register(radar);
        m_data->registerCallback(Board_dataCallback, NULL);
    }
}

void Board_run(void)
{
    DataAvian_run();

    /* Check polling communication interfaces for incoming requests. */
    ProtocolHandler_run();

    LedSequence_run();

    /*Watchdog reset*/
#ifdef WATCHDOG
    wdt_feed();
#endif
}
