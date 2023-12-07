/*
 * usb_serial.c
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

#include "usb_serial.h"
#include "cycfg.h"

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void usb_high_isr(void);
static void usb_medium_isr(void);
static void usb_low_isr(void);

/*******************************************************************************
* Global Variables
********************************************************************************/
/* USB Interrupt Configuration */
const cy_stc_sysint_t usb_high_interrupt_cfg =
{
    .intrSrc = (IRQn_Type) usb_interrupt_hi_IRQn,
    .intrPriority = 5U,
};
const cy_stc_sysint_t usb_medium_interrupt_cfg =
{
    .intrSrc = (IRQn_Type) usb_interrupt_med_IRQn,
    .intrPriority = 6U,
};
const cy_stc_sysint_t usb_low_interrupt_cfg =
{
    .intrSrc = (IRQn_Type) usb_interrupt_lo_IRQn,
    .intrPriority = 7U,
};

/* USBDEV context variables */
cy_stc_usbfs_dev_drv_context_t  usb_drvContext;
cy_stc_usb_dev_context_t        usb_devContext;
cy_stc_usb_dev_cdc_context_t    usb_cdcContext;


_Bool usb_serial_init(void)
{
    /* Initialize the USB device */
    Cy_USB_Dev_Init(USB_DEV_HW, &USB_DEV_config, &usb_drvContext,&usb_devices[0], &usb_devConfig, &usb_devContext);

    /* Initialize the CDC Class */
    Cy_USB_Dev_CDC_Init(&usb_cdcConfig, &usb_cdcContext, &usb_devContext);

    /* Initialize the USB interrupts */
    Cy_SysInt_Init(&usb_high_interrupt_cfg,   &usb_high_isr);
    Cy_SysInt_Init(&usb_medium_interrupt_cfg, &usb_medium_isr);
    Cy_SysInt_Init(&usb_low_interrupt_cfg,    &usb_low_isr);

    /* Enable the USB interrupts */
    NVIC_EnableIRQ(usb_high_interrupt_cfg.intrSrc);
    NVIC_EnableIRQ(usb_medium_interrupt_cfg.intrSrc);
    NVIC_EnableIRQ(usb_low_interrupt_cfg.intrSrc);

    /* Make device appear on the bus. This function call is blocking,
       it waits till the device enumerates */
    Cy_USB_Dev_Connect(true, CY_USB_DEV_WAIT_FOREVER, &usb_devContext);

   // TODO: check if everything gets initialized correctly

    return true;
}


/***************************************************************************
* Function Name: usb_high_isr
********************************************************************************
* Summary:
*  This function processes the high priority USB interrupts.
*
***************************************************************************/
static void usb_high_isr(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USB_DEV_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseHi(USB_DEV_HW),
                               &usb_drvContext);
}


/***************************************************************************
* Function Name: usb_medium_isr
********************************************************************************
* Summary:
*  This function processes the medium priority USB interrupts.
*
***************************************************************************/
static void usb_medium_isr(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USB_DEV_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseMed(USB_DEV_HW),
                               &usb_drvContext);
}


/***************************************************************************
* Function Name: usb_low_isr
********************************************************************************
* Summary:
*  This function processes the low priority USB interrupts.
*
**************************************************************************/
static void usb_low_isr(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USB_DEV_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseLo(USB_DEV_HW),
                               &usb_drvContext);
}
