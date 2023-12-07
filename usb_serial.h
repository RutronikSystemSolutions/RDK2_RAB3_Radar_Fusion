/*
 * usb_serial.h
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
#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_

#include "cy_usb_dev.h"
#include "cycfg_usbdev.h"

#define USB_BUFFER_SIZE 	(64u)
#define USB_SERIAL_PORT    	(0U)

/* USBDEV context variables */
extern cy_stc_usbfs_dev_drv_context_t  usb_drvContext;
extern  cy_stc_usb_dev_context_t        usb_devContext;
extern  cy_stc_usb_dev_cdc_context_t    usb_cdcContext;

_Bool usb_serial_init(void);

#endif /* USB_SERIAL_H_ */
