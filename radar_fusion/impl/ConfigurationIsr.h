#ifndef CONFIGURATIONISR_H
#define CONFIGURATIONISR_H

/* USB priority is hardcoded within Atmel ASF library under: UDD_USB_INT_LEVEL */
//#define ISR_PRIORITY_USB_IRQ    5

#define ISR_PRIORITY_SPI_IRQ           0
#define ISR_PRIORITY_GPIO_IRQ          1  // GPIO triggered interrupt (must be lower than QSPI and DMA due to call hierarchy)
#define ISR_PRIORITY_SYSTIMER_IRQ      10
#define ISR_PRIORITY_TIMER_COUNTER_IRQ 2


#endif
