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
#include <BoardDefinition.h>

#include "timer.h"

#include <impl/Platform.h>
#include <impl/PlatformData.h>
#include <impl/PlatformGpio.h>
#include <impl/PlatformI2c.h>
#include <impl/PlatformSpi.h>

#include <bsp/i2c.h>
#include <bsp/spi.h>

#include <common/typeutils.h>
#include <fatal_error.h>

/*RDK2 Platform Includes*/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cycfg.h"
#include "system_timer.h"
#include "impl/chrono.h"
#include "usb_serial.h"
#include "spi_interface.h"
#include "radar_irq.h"
#include "watchdog.h"


void PlatformInterfaces_swapI2cIds(void)
{
    if (ARRAY_SIZE_CHECKED(BoardI2cDefinition) != 2)
    {
        fatal_error(0);
    }

    PlatformI2cDefinition_t tmpI2c = BoardI2cDefinition[0];
    BoardI2cDefinition[0]          = BoardI2cDefinition[1];
    BoardI2cDefinition[1]          = tmpI2c;
}

void Platform_Constructor(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /*Initialize System Timer*/
    if(!system_timer_init())
    {
    	 CY_ASSERT(0);
    }

    /*Initialize LEDs*/
    result = cyhal_gpio_init( LED1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}
    result = cyhal_gpio_init( LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}

    /*Initialize BGT60TR13C Power Control pin*/
    result = cyhal_gpio_init(ARDU_IO3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false); /*Keep it OFF*/
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}

    /*Initialize BGT60TR13C RESET pin*/
    result = cyhal_gpio_init(RAD_RSTN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}

    /*Radar RESET*/
    CyDelay(100);
    cyhal_gpio_write(RAD_RSTN, false);
    CyDelay(20);
    cyhal_gpio_write(RAD_RSTN, true);

    /*Initialize NJR4652F2S2 RESET pin*/
    result = cyhal_gpio_init(ARDU_IO5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW, false); /*Keep it OFF*/
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}

    /*Initialize SPI Interface with Radar*/
    if(!spi_interface_init())
    {
    	CY_ASSERT(0);
    }

    /*Initialize Radar IRQ Pin*/
    if(!radar_irq_init())
    {
    	CY_ASSERT(0);
    }

    /*Initialize USB CDC*/
    if(!usb_serial_init())
    {
    	CY_ASSERT(0);
    }

    /*Initialize Watchdog*/
#ifdef WATCHDOG
    initialize_wdt();
#endif
}

void Platform_run(void)
{
}
