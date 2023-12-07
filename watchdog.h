/*
 * watchdog.h
 *
 *  Created on: 2023-12-07
 *      Author: Gintaras Drukteinis
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define WATCHDOG
#define WDT_TIME_OUT_MS             1000

void initialize_wdt(void);
void wdt_feed(void);

#endif /* WATCHDOG_H_ */
