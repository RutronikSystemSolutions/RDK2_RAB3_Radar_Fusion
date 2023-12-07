/*
 * system_timer.c
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

#include "cyhal.h"
#include "cybsp.h"
#include "system_timer.h"

/* System Timer object used */
cyhal_timer_t chrono_obj;
/*System Timer configuration*/
const cyhal_timer_cfg_t chrono_cfg =
{
    .compare_value = 0,                 /* Timer compare value, not used */
    .period = 0xFFFFFFFF-1,             /* Timer period*/
    .direction = CYHAL_TIMER_DIR_UP,    /* Timer counts up */
    .is_compare = false,                /* Don't use compare mode */
    .is_continuous = true,             	/* Run timer indefinitely */
    .value = 0                          /* Initial value of counter */
};

/*System Timer Initialization*/
_Bool system_timer_init(void)
{
	cy_rslt_t result;

    /*Initialize the timer counting up 1000 times per second*/
    result = cyhal_timer_init(&chrono_obj, NC, NULL);
    if (result != CY_RSLT_SUCCESS)
    { return false; }
    result = cyhal_timer_configure(&chrono_obj, &chrono_cfg);
    if (result != CY_RSLT_SUCCESS)
    { return false; }
    result = cyhal_timer_set_frequency(&chrono_obj, 1000);
    if (result != CY_RSLT_SUCCESS)
    { return false; }
    result = cyhal_timer_start(&chrono_obj);
    if (result != CY_RSLT_SUCCESS)
    { return false; }

    /**/
    return true;
}

uint32_t SysTimer_getTime(void)
{
	return cyhal_timer_read(&chrono_obj);
}
