#ifndef VENDORPROTOCOL_H_
#define VENDORPROTOCOL_H_ 1

#include <common/serialization.h>
#include <universal/protocol/protocol_definitions.h>

#include <stdint.h>


static inline uint8_t RequestHeader_bmReqType(const uint8_t buf[])
{
    return serialToHost8(buf + 0);
}

static inline uint8_t RequestHeader_bRequest(const uint8_t buf[])
{
    return serialToHost8(buf + 1);
}

static inline uint16_t RequestHeader_wValue(const uint8_t buf[])
{
    return serialToHost16(buf + 2);
}

static inline uint16_t RequestHeader_wIndex(const uint8_t buf[])
{
    return serialToHost16(buf + 4);
}

static inline uint16_t RequestHeader_wLength(const uint8_t buf[])
{
    return serialToHost16(buf + 6);
}

typedef struct
{
    uint8_t bmPktType;
    uint8_t bChannel;
    uint16_t wCounter;
    uint16_t wLength;
} VendorProtocol_DataPacketHeader;

//uint64_t timestamp (Am ende)


/**
 * Request's header (Little-endian format)
 */
typedef struct
{
    uint8_t bmReqType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} VendorProtocol_RequestHeader;

/**
 *  Response's header (Little-endian format)
 */
typedef struct
{
    uint8_t bmReqType;
    uint8_t bStatus;
    uint16_t wLength;
} VendorProtocol_ResponseHeader;


static inline uint8_t sizeof_RequestHeader(void)
{
    return sizeof(((VendorProtocol_RequestHeader *)0)->bmReqType) +
           sizeof(((VendorProtocol_RequestHeader *)0)->bRequest) +
           sizeof(((VendorProtocol_RequestHeader *)0)->wValue) +
           sizeof(((VendorProtocol_RequestHeader *)0)->wIndex) +
           sizeof(((VendorProtocol_RequestHeader *)0)->wLength);
}

static inline uint8_t sizeof_ResponseHeader(void)
{
    return sizeof(((VendorProtocol_ResponseHeader *)0)->bmReqType) +
           sizeof(((VendorProtocol_ResponseHeader *)0)->bStatus) +
           sizeof(((VendorProtocol_ResponseHeader *)0)->wLength);
}

static inline uint8_t sizeof_FrameHeader(void)
{
    return sizeof(((VendorProtocol_DataPacketHeader *)0)->bmPktType) +
           sizeof(((VendorProtocol_DataPacketHeader *)0)->bChannel) +
           sizeof(((VendorProtocol_DataPacketHeader *)0)->wCounter) +
           sizeof(((VendorProtocol_DataPacketHeader *)0)->wLength);
}


/** Unserialize request header from buffer
 *
 *  @param reqHeader a pointer to a RequestHeader
 *	@param buf a serialized request
 */
static inline void VendorProtocol_unserializeRequestHeader(VendorProtocol_RequestHeader *reqHeader, const uint8_t *buf)
{
    reqHeader->bmReqType = buf[0];
    reqHeader->bRequest  = buf[1];
    reqHeader->wValue    = (buf[3] << 8) | buf[2];
    reqHeader->wIndex    = (buf[5] << 8) | buf[4];
    reqHeader->wLength   = (buf[7] << 8) | buf[6];
}

/** Unserialize response header from buffer
 *
 *  @param respHeader a pointer to a ResponseHeader
 *  @param buf a serialized response
 */
static inline void VendorProtocol_unserializeResponseHeader(VendorProtocol_ResponseHeader *respHeader, const uint8_t *buf)
{
    respHeader->bmReqType = buf[0];
    respHeader->bStatus   = buf[1];
    respHeader->wLength   = (buf[3] << 8) | buf[2];
}

/** Serialize a request header into the specified buffer
 *
 *  @param reqHeader a pointer to a RequestHeader
 *  @param buf a serialized request
 */
static inline void VendorProtocol_serializeRequestHeader(const VendorProtocol_RequestHeader *reqHeader, uint8_t *buf)
{
    buf[0] = reqHeader->bmReqType;
    buf[1] = reqHeader->bRequest;
    buf[2] = reqHeader->wValue;
    buf[3] = reqHeader->wValue >> 8;
    buf[4] = reqHeader->wIndex;
    buf[5] = reqHeader->wIndex >> 8;
    buf[6] = reqHeader->wLength;
    buf[7] = reqHeader->wLength >> 8;
}

/** Serialize a packet header into the specified buffer
 *
 *  @param respHeader the packet's header information
 *	@param buf a buffer where the serialized packet's header will be written to
 */
static inline void VendorProtocol_serializeResponseHeader(const VendorProtocol_ResponseHeader *respHeader, uint8_t *buf)
{
    buf[0] = respHeader->bmReqType;
    buf[1] = respHeader->bStatus;
    buf[2] = respHeader->wLength;
    buf[3] = respHeader->wLength >> 8;
}

static inline void VendorProtocol_serializeFrameHeader(const VendorProtocol_DataPacketHeader *frameHeader, uint8_t buf[])
{
    hostToSerial8(buf + 0, frameHeader->bmPktType);
    hostToSerial8(buf + 1, frameHeader->bChannel);
    hostToSerial16(buf + 2, frameHeader->wCounter);
    hostToSerial16(buf + 4, frameHeader->wLength);
}

#endif /* VENDORPROTOCOL_H_ */
