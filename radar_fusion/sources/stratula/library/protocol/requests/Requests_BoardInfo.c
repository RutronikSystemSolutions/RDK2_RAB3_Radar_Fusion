/**
 * \file 	Requests_SystemInfo.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Requests_SystemInfo               System Information Commands
 * \brief           System Information related Commands.
 *
 * @{
 */
#include "Requests_BoardInfo.h"
#include <board/BoardInfo.h>
#include <common/errors.h>
#include <platform/ids/getUuid.h>
#include <universal/protocol/protocol_definitions.h>


/** Read out the board information
 *
 *  @param wValue  unused
 *  @param wIndex  unused
 *  @param wLength depending on length of BOARD_NAME
 *  @param payload buffer where the requested data is:
 *                 payload[0] = BOARD_VID (LSB)
 *                 payload[1] = BOARD_VID (MSB)
 *                 payload[2] = BOARD_PID (LSB)
 *                 payload[3] = BOARD_PID (MSB)
 *                 payload[4] to payload[n] = "BOARD_NAME" (including the terminator character '\0')
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_REQUEST_WLENGTH_INVALID if length is not 32
 */
static inline uint8_t Requests_getBoardInfo(uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    *payloadOut = (uint8_t *)&boardInfo;
    *wLengthOut = sizeof(boardInfo);

    return STATUS_SUCCESS;
}

static inline uint8_t Requests_getBoardInfoLegacy(uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != 32)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    *payload = (uint8_t *)&boardInfo;

    return STATUS_SUCCESS;
}

/** Read out the software version information
 *
 *  @param wValue  unused
 *  @param wIndex  unused
 *  @param wLength must be 16
 *  @param payload buffer where the requested data is:
 *                 payload[0] = VERSION_MAJOR (LSB)
 *                 payload[1] = VERSION_MAJOR (MSB)
 *                 payload[2] = VERSION_MINOR (LSB)
 *                 payload[3] = VERSION_MINOR (MSB)
 *                 payload[4] = VERSION_PATCH (LSB)
 *                 payload[5] = VERSION_PATCH (MSB)
 *                 payload[6] = VERSION_BUILD (LSB)
 *                 payload[7] = VERSION_BUILD (MSB)
 *                 payload[8] = PROTOCOL_VERSION_MAJOR (LSB)
 *                 payload[9] = PROTOCOL_VERSION_MAJOR (MSB)
 *                 payload[A] = PROTOCOL_VERSION_MINOR (LSB)
 *                 payload[B] = PROTOCOL_VERSION_MINOR (MSB)
 *                 payload[C] - payload[F] = reserved
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_REQUEST_WLENGTH_INVALID if length is not valid
 */
static inline uint8_t Requests_getVersionInfo(uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(versionInfo))
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    *payload = (uint8_t *)&versionInfo;

    return STATUS_SUCCESS;
}

static inline uint8_t Requests_getUuid(uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != UUID_LENGTH)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    return getUuid(*payload);
}

/** Read out the software version information
 *
 *  @param wValue  unused
 *  @param wIndex  unused
 *  @param wLength depends on length of version string
 *  @param payload buffer where the requested data is:
 *                 payload[0] to payload[n] = "EXTENDEND_VERSION" (char string including the terminator character '\0')
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_REQUEST_WLENGTH_INVALID if length is not valid
 */
static inline uint8_t Requests_getExtendedVersion(uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != 0)
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }
    *payloadOut = (uint8_t *)&extendedVersion;
    *wLengthOut = sizeof(extendedVersion);

    return STATUS_SUCCESS;
}


uint8_t Requests_BoardInfo_transfer(uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (wValue)
    {
        case 0:
            return Requests_getBoardInfo(wIndex, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case 3:
            return Requests_getExtendedVersion(wIndex, wLengthIn, payloadIn, wLengthOut, payloadOut);
        default:
            break;
    }
    return STATUS_REQUEST_WVALUE_INVALID;
}

uint8_t Requests_BoardInfo_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    switch (wValue)
    {
        case 0:
            return Requests_getBoardInfoLegacy(wIndex, wLength, payload);
            break;
        case 1:
            return Requests_getVersionInfo(wIndex, wLength, payload);
            break;
        case 2:
            return Requests_getUuid(wIndex, wLength, payload);
            break;
        default:
            break;
    }
    return STATUS_REQUEST_WVALUE_INVALID;
}

/*  @} */
