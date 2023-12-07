/*
 * watchdog.c
 *
 *  Created on: 2023-12-07
 *      Author: Gintaras Drukteinis
 */

#include "watchdog.h"

/* WDT object */
cyhal_wdt_t wdt_obj;

void initialize_wdt(void)
{
    cy_rslt_t result;

    /* Initialize the WDT */
    result = cyhal_wdt_init(&wdt_obj, WDT_TIME_OUT_MS);

    /* WDT initialization failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

void wdt_feed(void)
{
    /* Reset WDT */
    cyhal_wdt_kick(&wdt_obj);
}
