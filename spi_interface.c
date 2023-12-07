/*
 * spi_interface.c
 *
 *  Created on: 2023-11-17
 *      Author: Gintaras Drukteinis
 */
/*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*/
#include "spi_interface.h"
#include "radar_irq.h"

cyhal_spi_t spi_radar_obj;
spi_async_data_t spi_irq_data =
{
		.async = false,
		.irq_signal = false,
		.spi_lock = false,
		.read_buff = NULL,
		.callback = NULL,
		.arg = NULL,
		.slice = 0,
		.split = 0
};

_Bool spi_interface_init(void)
{
	cy_rslt_t result;

    if (cyhal_spi_init(&spi_radar_obj,
                       RAD_SPI_MOSI,
                       RAD_SPI_MISO,
                       RAD_SPI_SCLK,
                       NC,
                       NULL,
                       8,
                       CYHAL_SPI_MODE_00_MSB,
                       false) != CY_RSLT_SUCCESS)
    {
    	return false;
    }

    /*Initialize Chip Select Pin*/
    result = cyhal_gpio_init(RAD_SPI_CSN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    if (result != CY_RSLT_SUCCESS)
    {
    	return false;
    }

    /* Set the data rate to RAD_SPI_FREQUENCY Mbps */
    if (cyhal_spi_set_frequency(&spi_radar_obj, RAD_SPI_FREQUENCY) != CY_RSLT_SUCCESS)
    {
    	return false;
    }

    /* Register a callback function to be called when the interrupt fires */
    cyhal_spi_register_callback(&spi_radar_obj, (cyhal_spi_event_callback_t) spi_interrupt_callback, NULL);

    /* Enable the events that will trigger the call back function */
    cyhal_spi_enable_event(&spi_radar_obj, CYHAL_SPI_IRQ_DONE, 3, true);

    /*Init PASS*/
    return true;
}

void spi_cs_enable(void)
{
	cyhal_gpio_write(RAD_SPI_CSN, false);
}

void spi_cs_disable(void)
{
	cyhal_gpio_write(RAD_SPI_CSN, true);
}

void spi_interrupt_callback(void *arg, cyhal_spi_event_t event)
{
    (void)arg;
    if ((event & CYHAL_SPI_IRQ_DONE ) != 0u)
    {
        /* Transmission is complete. Handle Event */
		if(spi_irq_data.async)
		{
			if(spi_irq_data.slice == 0)
			{
				spi_cs_disable();
				(*spi_irq_data.callback)(spi_irq_data.arg);
				spi_irq_data.callback = NULL;
				spi_irq_data.arg = NULL;
				spi_irq_data.spi_lock = false;
				spi_irq_data.async = false;
				spi_irq_data.split = 0;
			}
			else
			{
				spi_irq_data.slice--;
				if(spi_irq_data.slice == 1 && spi_irq_data.split > 0)
				{
					spi_irq_data.read_buff = spi_irq_data.read_buff+spi_irq_data.split;
					cyhal_spi_transfer_async(&spi_radar_obj, NULL, 0, spi_irq_data.read_buff, spi_irq_data.split);
				}
				else
				{
					spi_irq_data.read_buff = spi_irq_data.read_buff+BURST_SLICE_SIZE;
					cyhal_spi_transfer_async(&spi_radar_obj, NULL, 0, spi_irq_data.read_buff, BURST_SLICE_SIZE);
				}
			}
		}
    }
}
