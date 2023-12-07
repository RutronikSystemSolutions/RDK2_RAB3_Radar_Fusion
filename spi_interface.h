/*
 * spi_interface.h
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

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define BURST_SLICE_SIZE           			(128)
#define RAD_SPI_FREQUENCY      				(12500000UL)
#define RAD_SPI_SCLK       					ARDU_CLK
#define RAD_SPI_MOSI       					ARDU_MOSI
#define RAD_SPI_MISO       					ARDU_MISO
#define RAD_SPI_CSN        					ARDU_CS
#define RAD_RSTN           					ARDU_IO4

typedef struct spi_async
{
	uint8_t *read_buff;
	uint32_t read_lenght;
	void (*callback)(void *);
	void *arg;
	_Bool spi_lock;
	_Bool async;
	_Bool irq_signal;
	uint8_t slice;
	uint8_t split;

}spi_async_data_t;

extern spi_async_data_t spi_irq_data;
extern cyhal_spi_t spi_radar_obj;
_Bool spi_interface_init(void);
void spi_cs_enable(void);
void spi_cs_disable(void);
void spi_interrupt_callback(void *arg, cyhal_spi_event_t event);

#endif /* SPI_INTERFACE_H_ */
