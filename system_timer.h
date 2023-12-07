/*
 * system_timer.h
 *
 *  Created on: 2023-11-16
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
#ifndef SYSTEM_TIMER_H_
#define SYSTEM_TIMER_H_

#include "cyhal_hw_types.h"

#define TICK_TIME_MICROSECS		1000

extern cyhal_timer_t chrono_obj;

_Bool system_timer_init(void);
uint32_t SysTimer_getTime(void);


#endif /* SYSTEM_TIMER_H_ */
