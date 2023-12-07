/**
 * \file 	ProtocolHandler.c
 *
 * \addtogroup      CommunicationInterface      Communication Interface (Command-based)
 * @{
 *   \addtogroup      ProtocolHandler             Protocol Handler
 *   \brief 			Communication protocol handler.
 *   @{
 */
#include "ProtocolHandler.h"

#include <BoardDefinition.h>

#ifdef COMMUNICATION_SERIAL
#include "ProtocolHandlerSerial.h"
#endif

#ifdef COMMUNICATION_ETHERNET
#include "ethernet/ProtocolHandlerEthernet.h"
#endif

#ifdef COMMUNICATION_HSSL
#include <impl/hssl/ProtocolHandlerHssl.h>
#endif

#include "VendorProtocol.h"

#include <stdbool.h>

/******************************************************************************/
/* Type Definitions ----------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
static bool m_sendingDataFrames;
static uint8_t m_dataPacketInterface;
static uint8_t m_requestInterface;
static uint16_t m_maxDataPacketSize;
static VendorProtocol_DataPacketHeader m_dataPacketHeader;

static uint32_t errorData[][2] = {
    {0, sizeof(uint32_t)},
};

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/
#define INTERFACE_NONE     (0)
#define INTERFACE_ETHERNET (1)
#define INTERFACE_SERIAL   (2)

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/* Public Methods Definition -----------------------------------------------*/
/******************************************************************************/
void ProtocolHandler_Constructor(void)
{
#ifdef COMMUNICATION_ETHERNET
    ProtocolHandlerEthernet_Constructor();
#endif
#ifdef COMMUNICATION_SERIAL
    ProtocolHandlerSerial_Constructor();
#endif
#ifdef COMMUNICATION_HSSL
    ProtocolHandlerHssl_Constructor();
#endif
    m_sendingDataFrames   = false;
    m_dataPacketInterface = INTERFACE_NONE;
    m_requestInterface    = INTERFACE_NONE;
    m_maxDataPacketSize   = 0;
}

void ProtocolHandler_run(void)
{
#ifdef COMMUNICATION_SERIAL
    ProtocolHandlerSerial_run();
#endif
#ifdef COMMUNICATION_HSSL
    ProtocolHandlerHssl_processRequest();
#endif
}

sr_t ProtocolHandler_sendDataFrameArray(uint32_t data[][2], uint8_t count, uint8_t channel, uint64_t timestamp)
{
#if defined(COMMUNICATION_ETHERNET) || defined(COMMUNICATION_SERIAL)

    // check if we have empty data sets at the end
    for (uint_fast8_t i = count - 1u; i > 0; i--)
    {
        const uint32_t length = data[i][1];
        if (length == 0)
        {
            count = i;
        }
        else
        {
            break;
        }
    }

    const uint16_t maxPacketLength = m_maxDataPacketSize - sizeof_FrameHeader();

    m_dataPacketHeader.bChannel    = channel;
    const uint32_t timestampLength = timestamp ? sizeof(timestamp) : 0;

    // check if this is a single packet or an error frame
    if ((count < 2) && (((data[0][1] + timestampLength) <= maxPacketLength) || (data[0][0] == 0) || data[0][1] == 0))
    {
        m_dataPacketHeader.bmPktType = DATA_FRAME_SINGLE_PACKET;
        if (timestamp)
        {
            m_dataPacketHeader.bmPktType |= DATA_FRAME_FLAG_TIMESTAMP;
        }

        // check if this is an error frame
        if (data[0][0] == 0)
        {
            m_dataPacketHeader.bmPktType |= DATA_FRAME_FLAG_ERROR;
            errorData[0][0] = (uint32_t)(uintptr_t)&data[0][1];
            data            = errorData;
        }

        // check if this is a debug frame
        if (data[0][1] == 0)
        {
            m_dataPacketHeader.bmPktType |= DATA_FRAME_FLAG_ERROR;
            data[0][1] = strlen((char *)(uintptr_t)data[0][0]);
            if (data[0][1] > maxPacketLength)
            {
                data[0][1] = maxPacketLength;
            }
            if (data[0][1] == sizeof(uint32_t))
            {
                data[0][1] = sizeof(uint32_t) + 1;  // if it has the size of an error code, include the null-termination to distinguish it
            }
            count = 1;  // had been reset to zero by the empty data-set check above
        }
    }
    else
    {
        m_dataPacketHeader.bmPktType = DATA_FRAME_FIRST_PACKET;
    }

    for (uint_fast8_t i = 0; i < count; i++)
    {
        const uint8_t *payload = (const uint8_t *)(uintptr_t)data[i][0];
        uint32_t length        = data[i][1];
        if (i == (count - 1u))
        {
            if (timestamp)
            {
                length += sizeof(timestamp);
            }
        }
        while (length)
        {
            if (!m_sendingDataFrames)
            {
                return E_ABORTED;
            }

            m_dataPacketHeader.wLength = (length > maxPacketLength) ? maxPacketLength : length;
#ifdef COMMUNICATION_ETHERNET
            if (m_dataPacketInterface == INTERFACE_ETHERNET)
            {
                RETURN_ON_ERROR(ProtocolHandlerEthernet_sendDataPacket(&m_dataPacketHeader, payload, timestamp));
            }
            else
#endif
#ifdef COMMUNICATION_SERIAL
                if (m_dataPacketInterface == INTERFACE_SERIAL)
            {
                RETURN_ON_ERROR(ProtocolHandlerSerial_sendDataPacket(&m_dataPacketHeader, payload, timestamp));
            }
            else
#endif
            {
                return E_NOT_INITIALIZED;
            }

            //next packet
            m_dataPacketHeader.wCounter++;
            length -= m_dataPacketHeader.wLength;
            payload += m_dataPacketHeader.wLength;
            if ((i == (count - 1u)) && (length <= maxPacketLength))
            {
                m_dataPacketHeader.bmPktType = DATA_FRAME_LAST_PACKET;
                if (timestamp)
                {
                    m_dataPacketHeader.bmPktType |= DATA_FRAME_FLAG_TIMESTAMP;
                }
            }
            else
            {
                m_dataPacketHeader.bmPktType = DATA_FRAME_MIDDLE_PACKET;
            }
        }
    }

    return E_SUCCESS;
#else
    return E_NOT_POSSIBLE;
#endif
}

sr_t ProtocolHandler_sendDataFrame(const uint8_t *payload, uint32_t size, uint8_t channel, uint64_t timestamp)
{
    uint32_t data[][2] = {
        {(uint32_t)(uintptr_t)payload, size},
    };

    return ProtocolHandler_sendDataFrameArray(data, 1, channel, timestamp);
}

sr_t ProtocolHandler_sendErrorFrame(uint32_t code, uint8_t channel, uint64_t timestamp)
{
    uint32_t data[][2] = {
        {0, code},
    };

    return ProtocolHandler_sendDataFrameArray(data, 1, channel, timestamp);
}

sr_t ProtocolHandler_sendDebugFrameImpl(char *message, uint8_t channel, uint64_t timestamp)
{
    uint32_t data[][2] = {
        {(uint32_t)(uintptr_t)message, 0},
    };

    return ProtocolHandler_sendDataFrameArray(data, 1, channel, timestamp);
}

void ProtocolHandler_setRequestInterfaceSerial(void)
{
    m_requestInterface = INTERFACE_SERIAL;
}

void ProtocolHandler_setRequestInterfaceEthernet(void)
{
    m_requestInterface = INTERFACE_ETHERNET;
}

void ProtocolHandler_initializeSendingDataFrames(void)
{
    m_sendingDataFrames         = true;
    m_dataPacketInterface       = m_requestInterface;
    m_dataPacketHeader.wCounter = 0;
#ifdef COMMUNICATION_ETHERNET
    if (m_dataPacketInterface == INTERFACE_ETHERNET)
    {
        m_maxDataPacketSize = PROTOCOL_HANDLER_ETHERNET_MAX_PACKET_SIZE;
    }
    else
#endif
#ifdef COMMUNICATION_SERIAL
        if (m_dataPacketInterface == INTERFACE_SERIAL)
    {
        m_maxDataPacketSize = SERIAL_MAX_PACKET_SIZE - PROTOCOL_HANDLER_SERIAL_CRC_SIZE;
    }
    else
#endif
    {
        m_maxDataPacketSize = 0;
    }
}

void ProtocolHandler_resetSendingDataFrames(void)
{
    m_sendingDataFrames = false;
#ifdef COMMUNICATION_SERIAL
    if (m_dataPacketInterface == INTERFACE_SERIAL)
    {
        ProtocolHandlerSerial_reset();
    }
#endif
}

/* @}@} */
