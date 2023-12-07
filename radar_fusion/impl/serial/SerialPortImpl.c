/* ===========================================================================
** Copyright (C) 2021 Infineon Technologies AG
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
** ===========================================================================
*/
/*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*/

#include <platform/impl/SerialPortImpl.h>
#include <impl/chrono.h>
#include <string.h>
#include "usb_serial.h"

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/
static uint16_t com_get_data(void *data, uint16_t num_bytes)
{
	uint32_t count = 0;
	chrono_ticks_t deadline = chrono_get_timepoint(chrono_milliseconds(1));

	while(count != num_bytes)
	{
		count = Cy_USB_Dev_CDC_GetCount(USB_SERIAL_PORT, &usb_cdcContext);
		if (chrono_has_passed(deadline))
		{
			if(count > 0)
			{
				return (uint16_t)Cy_USB_Dev_CDC_GetData(USB_SERIAL_PORT, data, num_bytes, &usb_cdcContext);
			}
			break;
		}
	}

	return 0;
}

/******************************************************************************/
/* Public Methods Definition -----------------------------------------------*/
/******************************************************************************/
void SerialPort_Constructor(void)
{
}

sr_t SerialPort_open(uint32_t baudrate)
{
    return E_SUCCESS;
}

bool SerialPort_isOpened(void)
{
    return true;
}

sr_t SerialPort_close(void)
{
    return E_SUCCESS;
}

void SerialPort_clearInputBuffer(void)
{
    uint8_t rxBuffer[USB_BUFFER_SIZE];

	if (Cy_USB_Dev_CDC_IsDataReady(USB_SERIAL_PORT, &usb_cdcContext))
	{
		Cy_USB_Dev_CDC_GetAll(USB_SERIAL_PORT, rxBuffer, USB_BUFFER_SIZE, &usb_cdcContext);
	}
}

void SerialPort_flushOutputBuffer(void)
{
}

sr_t SerialPort_send(const uint8_t data[], uint16_t length)
{
	int32_t buff_size = length;

	if(buff_size < USB_BUFFER_SIZE)
	{
		/* Wait until component is ready to send data to host */
		 while (0u == Cy_USB_Dev_CDC_IsReady(USB_SERIAL_PORT, &usb_cdcContext))
		 {}

		/*Send Data*/
		Cy_USB_Dev_CDC_PutData(USB_SERIAL_PORT, data, length, &usb_cdcContext);
	}
	else
	{
		 uint32_t j = buff_size / USB_BUFFER_SIZE;
		 if(buff_size % USB_BUFFER_SIZE != 0)
		 {
			 j++;
		 }
		 for(uint32_t i = 0; i < j; i++)
		 {
				/* Wait until component is ready to send data to host */
				 while (0u == Cy_USB_Dev_CDC_IsReady(USB_SERIAL_PORT, &usb_cdcContext))
				 {}

			 if(buff_size > USB_BUFFER_SIZE)
			 {
				 Cy_USB_Dev_CDC_PutData(USB_SERIAL_PORT, data, USB_BUFFER_SIZE, &usb_cdcContext);
				 data = data+USB_BUFFER_SIZE;
				 buff_size = buff_size - USB_BUFFER_SIZE;
			 }
			 else if (buff_size != 0)
			 {
				 Cy_USB_Dev_CDC_PutData(USB_SERIAL_PORT, data, (uint32_t)buff_size, &usb_cdcContext);
				 data = data+buff_size;
			 }

		 }
	}

    return E_SUCCESS;
}

sr_t SerialPort_receive(uint8_t data[], uint16_t length, uint16_t timeout, bool returnImmediately, uint16_t *count)
{
    sr_t ret                      = E_SUCCESS;
    uint16_t left                 = length;
    const chrono_ticks_t deadline = chrono_get_timepoint(chrono_milliseconds(timeout));
    while (left > 0)
    {
        const uint16_t numReceivedBytes = com_get_data(data, left);
        if ((numReceivedBytes == 0) && returnImmediately)
        {
            break;
        }

        data += numReceivedBytes;
        left -= numReceivedBytes;

        if (chrono_has_passed(deadline))
        {
            ret = E_TIMEOUT;
            break;
        }
    }

    *count = (length - left);
    return ret;
}

sr_t SerialPort_sendString(const char data[])
{
    return SerialPort_send((uint8_t *)data, strlen(data));
}
