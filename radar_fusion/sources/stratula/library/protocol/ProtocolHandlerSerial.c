#include "ProtocolHandlerSerial.h"
#include "ProtocolHandler.h"
#include "RequestHandler.h"
#include <platform/impl/SerialPortImpl.h>

#include <common/crc/crc16.h>
#include <common/errors.h>
#include <common/typeutils.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************/
/* Type Definitions ----------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private/Public Variables ---------------------------------------------------*/
/******************************************************************************/
ALIGNED_BUFFER(uint8_t, m_buffer, uint32_t, SERIAL_MAX_PACKET_SIZE);
static bool m_serialPortEnabled;
static uint16_t m_crc;
static volatile bool m_requestActive;

/******************************************************************************/
/*Private/Public Constants ---------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Declaration ------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*Private Methods Definition -------------------------------------------------*/
/******************************************************************************/

static sr_t ProtocolHandlerSerial_sendPayload(const uint8_t *payload, uint16_t length)
{
    m_crc = crc16_ccitt_false(payload, length, m_crc);
    return SerialPort_send(payload, length);
}

static sr_t ProtocolHandlerSerial_sendCrc(void)
{
    m_buffer[0] = m_crc >> 8;
    m_buffer[1] = m_crc;
    RETURN_ON_ERROR(SerialPort_send(m_buffer, PROTOCOL_HANDLER_SERIAL_CRC_SIZE));

    SerialPort_flushOutputBuffer();
    return E_SUCCESS;
}

static void ProtocolHandlerSerial_response(uint8_t bmReqType, uint8_t bStatus, uint16_t wLength, const uint8_t *payload)
{
    //send response-header
    VendorProtocol_ResponseHeader respHeader;
    respHeader.bmReqType = bmReqType;
    respHeader.bStatus   = bStatus;
    respHeader.wLength   = wLength;
    VendorProtocol_serializeResponseHeader(&respHeader, m_buffer);
    SerialPort_send(m_buffer, sizeof_ResponseHeader());
    m_crc = crc16_ccitt_false(m_buffer, sizeof_ResponseHeader(), CRC16_CCITT_FALSE_SEED);

    if (wLength > 0)
    {
        ProtocolHandlerSerial_sendPayload(payload, wLength);
    }

    ProtocolHandlerSerial_sendCrc();
}

static uint8_t ProtocolHandlerSerial_parseInputData(VendorProtocol_RequestHeader *request, uint8_t **requestPayload)
{
    //Check header
    VendorProtocol_unserializeRequestHeader(request, m_buffer);
    uint16_t headerSize;
    switch (request->bmReqType)
    {
        case VENDOR_REQ_WRITE:
        case VENDOR_REQ_TRANSFER:
            headerSize = sizeof_RequestHeader();
            break;
        case VENDOR_REQ_READ:
            headerSize = sizeof_ResponseHeader();
            break;
        default:
            headerSize = sizeof_RequestHeader();
            break;
    }
    if (request->wLength > (SERIAL_MAX_PACKET_SIZE - headerSize - PROTOCOL_HANDLER_SERIAL_CRC_SIZE))
    {
        //Discard payload and crc
        SerialPort_clearInputBuffer();
        return STATUS_PAYLOAD_TOO_LONG;
    }

    if ((request->wLength != 0) && (requestPayload != NULL))
    {
        //Check payload
        *requestPayload = m_buffer + sizeof_RequestHeader();
        uint16_t wLength;
        SerialPort_receive(*requestPayload, request->wLength, PROTOCOL_HANDLER_SERIAL_TIMEOUT, false, &wLength);
        if (request->wLength != wLength)
        {
            return STATUS_PAYLOAD_INCOMPLETE;
        }
    }

    //Check crc
    uint8_t *crcBuf = m_buffer + (SERIAL_MAX_PACKET_SIZE - PROTOCOL_HANDLER_SERIAL_CRC_SIZE);
    uint16_t crcSize;
    SerialPort_receive(crcBuf, PROTOCOL_HANDLER_SERIAL_CRC_SIZE, PROTOCOL_HANDLER_SERIAL_TIMEOUT, false, &crcSize);
    if (PROTOCOL_HANDLER_SERIAL_CRC_SIZE != crcSize)
    {
        return STATUS_CRC_ERROR;
    }
    uint16_t crc = crc16_ccitt_false(m_buffer, sizeof_RequestHeader(), CRC16_CCITT_FALSE_SEED);
    if ((request->wLength != 0) && (requestPayload != NULL))
    {
        crc = crc16_ccitt_false(*requestPayload, request->wLength, crc);
    }
    if (crc16_ccitt_false(crcBuf, PROTOCOL_HANDLER_SERIAL_CRC_SIZE, crc))
    {
        return STATUS_CRC_ERROR;
    }

    return STATUS_SUCCESS;
}

static void ProtocolHandlerSerial_write(void)
{
    VendorProtocol_RequestHeader request;
    uint8_t *requestPayload = NULL;

    const uint8_t parseResult = ProtocolHandlerSerial_parseInputData(&request, &requestPayload);
    if (parseResult != STATUS_SUCCESS)
    {
        ProtocolHandlerSerial_response(VENDOR_REQ_WRITE, parseResult, 0, NULL);
        return;
    }

    //Execute write-request
    const uint8_t bStatus = RequestHandler_write(
        request.bRequest,
        request.wValue,
        request.wIndex,
        request.wLength,
        requestPayload);
    ProtocolHandlerSerial_response(VENDOR_REQ_WRITE, bStatus, 0, NULL);
}

static void ProtocolHandlerSerial_read(void)
{
    //Check header
    VendorProtocol_RequestHeader request;
    const uint8_t parseResult = ProtocolHandlerSerial_parseInputData(&request, NULL);
    if (parseResult != STATUS_SUCCESS)
    {
        ProtocolHandlerSerial_response(VENDOR_REQ_READ, parseResult, 0, NULL);
        return;
    }

    //Execute read-request
    uint8_t *responsePayload = m_buffer + sizeof_ResponseHeader();
    const uint8_t bStatus    = RequestHandler_read(request.bRequest,
                                                   request.wValue,
                                                   request.wIndex,
                                                   request.wLength,
                                                   &responsePayload);

    //Do not send the responsePayload if read fails
    uint16_t wLength = (bStatus == STATUS_SUCCESS) ? request.wLength : 0;
    ProtocolHandlerSerial_response(VENDOR_REQ_READ, bStatus, wLength, responsePayload);
}

static void ProtocolHandlerSerial_transfer(void)
{
    VendorProtocol_RequestHeader request;
    uint8_t *requestPayload = NULL;

    const uint8_t parseResult = ProtocolHandlerSerial_parseInputData(&request, &requestPayload);
    if (parseResult != STATUS_SUCCESS)
    {
        ProtocolHandlerSerial_response(VENDOR_REQ_TRANSFER, parseResult, 0, NULL);
        return;
    }

    uint8_t *responsePayload = m_buffer + sizeof_ResponseHeader();
    uint16_t wResponseLength;

    //Execute transfer-request
    const uint8_t bStatus = RequestHandler_transfer(
        request.bRequest,
        request.wValue,
        request.wIndex,
        request.wLength,
        requestPayload,
        &wResponseLength,
        &responsePayload);

    //Do not send the responsePayload if read fails
    if (bStatus != STATUS_SUCCESS)
    {
        wResponseLength = 0;
    }
    ProtocolHandlerSerial_response(VENDOR_REQ_TRANSFER, bStatus, wResponseLength, responsePayload);
}

/******************************************************************************/
/* Public Methods Definition -----------------------------------------------*/
/******************************************************************************/
void ProtocolHandlerSerial_Constructor(void)
{
    m_requestActive = false;
    SerialPort_Constructor();
    SerialPort_open(PROTOCOL_HANDLER_SERIAL_BAUDRATE);
    m_serialPortEnabled = SerialPort_isOpened();
}

void ProtocolHandlerSerial_run(void)
{
    if (!m_serialPortEnabled)
    {
        return;
    }
    //Check for incoming request
    uint16_t reqSize;
    SerialPort_receive(m_buffer, sizeof_RequestHeader(), PROTOCOL_HANDLER_SERIAL_TIMEOUT, true, &reqSize);
    if (reqSize == 0)
    {
        return;
    }

    m_requestActive = true;

    ProtocolHandler_setRequestInterfaceSerial();

    const uint8_t bmReqType = m_buffer[0];
    if (reqSize < sizeof_RequestHeader())
    {
        ProtocolHandlerSerial_response(bmReqType, STATUS_HEADER_INCOMPLETE, 0, NULL);
        return;
    }

    //Read remaining request (payload + crc)
    if (bmReqType == VENDOR_REQ_WRITE)
    {
        ProtocolHandlerSerial_write();
    }
    else if (bmReqType == VENDOR_REQ_READ)
    {
        ProtocolHandlerSerial_read();
    }
    else if (bmReqType == VENDOR_REQ_TRANSFER)
    {
        ProtocolHandlerSerial_transfer();
    }
    else
    {
        ProtocolHandlerSerial_response(bmReqType, STATUS_REQUEST_TYPE_INVALID, 0, NULL);
    }

    m_requestActive = false;
}

sr_t ProtocolHandlerSerial_sendDataPacket(VendorProtocol_DataPacketHeader *header, const uint8_t *payload, uint64_t timestamp)
{
    if (!m_serialPortEnabled)
    {
        return E_NOT_ALLOWED;
    }

    while (m_requestActive)
        ;

    VendorProtocol_serializeFrameHeader(header, m_buffer);
    RETURN_ON_ERROR(SerialPort_send(m_buffer, sizeof_FrameHeader()));
    m_crc = crc16_ccitt_false(m_buffer, sizeof_FrameHeader(), CRC16_CCITT_FALSE_SEED);

    if (header->bmPktType & DATA_FRAME_FLAG_TIMESTAMP)
    {
        const uint16_t payloadLength = header->wLength - sizeof(timestamp);
        RETURN_ON_ERROR(ProtocolHandlerSerial_sendPayload(payload, payloadLength));
        RETURN_ON_ERROR(ProtocolHandlerSerial_sendPayload((const uint8_t *)&timestamp, sizeof(timestamp)));  // time stamp is assumed to be little-endian
    }
    else
    {
        RETURN_ON_ERROR(ProtocolHandlerSerial_sendPayload(payload, header->wLength));
    }

    RETURN_ON_ERROR(ProtocolHandlerSerial_sendCrc());

    ProtocolHandlerSerial_run();  // check if measurements should be stopped while sending a frame

    return E_SUCCESS;
}

void ProtocolHandlerSerial_reset(void)
{
    SerialPort_flushOutputBuffer();
}
