/*
 * radar_irq.h
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
#ifndef RADAR_IRQ_H_
#define RADAR_IRQ_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define RAD_IRQ_PRIORITY					(4)
#define GPIO_INTERRUPT_PRIORITY            	(6)
#define RAD_IRQ            					ARDU_IO6

extern void (*func)(void*);
extern void *rad_arg;

void radar_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event);
_Bool radar_irq_init(void);


#endif /* RADAR_IRQ_H_ */
