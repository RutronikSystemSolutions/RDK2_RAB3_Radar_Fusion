/**
 * \file 	RequestHandler.c
 *
 * \addtogroup      CommunicationInterface      Communication Interface (Command-based)
 * @{
 *   \addtogroup      RequestHandler              Request Handler
 *   \brief           Interprets and executes commands from remote host.
 *   @{
 */
#include "RequestHandler.h"
#include "CommandHandler.h"
#include <impl/custom/RequestHandlerCustom.h>
#include <universal/protocol/protocol_definitions.h>

//Include all command-functions
#include "requests/Requests_BoardInfo.h"
#include "requests/Requests_ErrorInfo.h"
#include "requests/Requests_IData.h"
#include "requests/Requests_IGpio.h"
#include "requests/Requests_II2c.h"
#include "requests/Requests_IMemory.h"
#include "requests/Requests_ISpi.h"

#include <stddef.h>

/******************************************************************************/
/*Macro Definitions ----------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
static CommandHandler m_commandHandlerComponents;
static CommandHandler m_commandHandlerModules;

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/* Public Methods Definition -----------------------------------------------*/
/******************************************************************************/
static IGpio *m_gpio;
static ISpi *m_spi;
static IData *m_data;
static II2c *m_i2c;

void RequestHandler_Constructor(IGpio *gpio, ISpi *spi, IData *data, II2c *i2c)
{
    m_gpio = gpio;
    m_spi  = spi;
    m_data = data;
    m_i2c  = i2c;
    CommandHandler_Constructor(&m_commandHandlerComponents);
    CommandHandler_Constructor(&m_commandHandlerModules);
}

bool RequestHandler_registerComponentImplementation(const ICommands *commands)
{
    return CommandHandler_registerImplementation(&m_commandHandlerComponents, commands);
}

bool RequestHandler_registerModuleImplementation(const ICommands *commands)
{
    return CommandHandler_registerImplementation(&m_commandHandlerModules, commands);
}

uint8_t RequestHandler_write(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    switch (bRequest)
    {
        case REQ_MEMORY:
            return Requests_IMemory_write(wValue, wIndex, wLength, payload);
            break;
        case REQ_GPIO:
            if (m_gpio == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_IGpio_write(m_gpio, wValue, wIndex, wLength, payload);
            break;
        case REQ_I2C:
            if (m_i2c == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_II2c_write(m_i2c, wValue, wIndex, wLength, payload);
            break;
        case REQ_I2C_TRANSACTION_16:
            if (m_i2c == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_II2c_transaction16_write(m_i2c, wValue, wIndex, wLength, payload);
            break;
        case REQ_SPI:
            if (m_spi == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_ISpi_write(m_spi, wValue, wIndex, wLength, payload);
            break;
        case REQ_DATA:
            if (m_data == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_IData_write(m_data, wValue, wIndex, wLength, payload);
            break;
        case REQ_MEMORY_STREAM:
            return Requests_IMemory_stream(wValue, wIndex, wLength, payload);
            break;
        case CMD_COMPONENT:
            return CommandHandler_write(&m_commandHandlerComponents, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLength, payload);
            break;
        case CMD_MODULE:
            return CommandHandler_write(&m_commandHandlerModules, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLength, payload);
            break;
        case REQ_CUSTOM:
            return RequestHandlerCustom_write(wValue, wIndex, wLength, payload);
            break;
        default:
            break;
    }
    return STATUS_REQUEST_INVALID;
}

uint8_t RequestHandler_read(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    switch (bRequest)
    {
        case REQ_BOARD_INFO:
            return Requests_BoardInfo_read(wValue, wIndex, wLength, payload);
            break;
        case REQ_ERROR_INFO:
            return Requests_ErrorInfo_read(wValue, wIndex, wLength, payload);
            break;
        case REQ_MEMORY:
            return Requests_IMemory_read(wValue, wIndex, wLength, payload);
            break;
        case REQ_GPIO:
            if (m_gpio == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_IGpio_read(m_gpio, wValue, wIndex, wLength, payload);
            break;
        case REQ_I2C:
            if (m_i2c == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_II2c_read(m_i2c, wValue, wIndex, wLength, payload);
            break;
        case REQ_I2C_TRANSACTION_16:
            if (m_i2c == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_II2c_transaction16_read(m_i2c, wValue, wIndex, wLength, payload);
            break;
        case REQ_SPI:
            if (m_spi == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_ISpi_read(m_spi, wValue, wIndex, wLength, payload);
            break;
        case REQ_DATA:
            if (m_data == NULL)
            {
                return STATUS_REQUEST_NOT_AVAILABLE;
            }
            return Requests_IData_read(m_data, wValue, wIndex, wLength, payload);
            break;
        case CMD_COMPONENT:
            return CommandHandler_read(&m_commandHandlerComponents, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLength, payload);
            break;
        case CMD_MODULE:
            return CommandHandler_read(&m_commandHandlerModules, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLength, payload);
            break;
        case REQ_CUSTOM:
            return RequestHandlerCustom_read(wValue, wIndex, wLength, payload);
            break;
        default:
            break;
    }
    return STATUS_REQUEST_INVALID;
}

uint8_t RequestHandler_transfer(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bRequest)
    {
        case REQ_BOARD_INFO:
            return Requests_BoardInfo_transfer(wValue, wIndex, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case REQ_SPI:
            return Requests_ISpi_transfer(m_spi, wValue, wIndex, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case CMD_COMPONENT:
            return CommandHandler_transfer(&m_commandHandlerComponents, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case CMD_MODULE:
            return CommandHandler_transfer(&m_commandHandlerModules, CMD_GET_TYPE(wValue), CMD_GET_IMPL(wValue), CMD_GET_ID(wIndex), CMD_GET_SUBIF(wIndex), CMD_GET_FUNCTION(wIndex), wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case REQ_CUSTOM:
            return RequestHandlerCustom_transfer(wValue, wIndex, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            break;
    }
    return STATUS_REQUEST_INVALID;
}

/* @}@} */
