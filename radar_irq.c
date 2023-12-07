/*
 * radar_irq.c
 *
 *  Created on: 2023-12-04
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
#include "radar_irq.h"

cyhal_gpio_callback_data_t rad_irq_data =
{
		.callback = radar_interrupt_handler,
		.callback_arg = NULL,
};

/*Radar Interrupt Callback Function*/
void (*func)(void*) = NULL;
void *rad_arg = NULL;

/* Interrupt handler callback function */
void radar_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event)
{
	CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);

    if(!func || !rad_arg)
    {
    	return;
    }

    (*func)(rad_arg);
}

_Bool radar_irq_init(void)
{
	cy_rslt_t result;

    /*Initialize Radar IRQ Pin*/
    result = cyhal_gpio_init(RAD_IRQ, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    if (result != CY_RSLT_SUCCESS)
    {
    	return false;
    }

    /*Register callback functions */
    cyhal_gpio_register_callback(RAD_IRQ, &rad_irq_data);

    /* Enable/Disable falling edge interrupt events */
    cyhal_gpio_enable_event(RAD_IRQ, CYHAL_GPIO_IRQ_RISE, RAD_IRQ_PRIORITY, false);

    /*Init PASS*/
    return true;
}

