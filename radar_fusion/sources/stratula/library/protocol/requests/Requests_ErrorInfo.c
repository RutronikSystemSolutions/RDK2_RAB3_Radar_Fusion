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
#include "Requests_ErrorInfo.h"
#include <common/errors.h>
#include <universal/protocol/protocol_definitions.h>

#include <string.h>


/** Read out the errors from errors.h
 *
 *  @param wValue  unused
 *  @param wIndex  unused
 *  @param wLength must be 16
 *  @param payload buffer where the requested data is:
 *                 payload[0] = error_platform       (LSB)
 *                 payload[1] = error_platform       (Byte 1)
 *                 payload[2] = error_platform       (Byte 2)
 *                 payload[3] = error_platform       (MSB)
 *                 payload[4] = error_component      (LSB)
 *                 ...
 *                 payload[7] = error_component      (MSB)
 *                 payload[8] = error_module          (LSB)
 *                 ...
 *                 payload[11] = error_module         (MSB)
 *                 payload[12] = error_communication (LSB)
 *                 ...
 *                 payload[15] = error_communication (MSB)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_REQUEST_WLENGTH_INVALID if length is not valid
 */
static inline uint8_t Requests_getDetailedError(uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    if (wLength != 4 * sizeof(int))
    {
        return STATUS_REQUEST_WLENGTH_INVALID;
    }

    int dummy = 0;  //dummy variable replacing the old global error_xxx variables

    memcpy(*payload + sizeof(int) * 0, &dummy, sizeof(dummy));  //was error_platform
    memcpy(*payload + sizeof(int) * 1, &dummy, sizeof(dummy));  //was error_component
    memcpy(*payload + sizeof(int) * 2, &dummy, sizeof(dummy));  //was error_module
    memcpy(*payload + sizeof(int) * 3, &dummy, sizeof(dummy));  //was error_communication

    return STATUS_SUCCESS;
}

uint8_t Requests_ErrorInfo_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    switch (wValue)
    {
        case 0:
            return STATUS_REQUEST_NOT_AVAILABLE;
            break;
        case 1:
            return Requests_getDetailedError(wIndex, wLength, payload);
            break;
        default:
            break;
    }
    return STATUS_REQUEST_WVALUE_INVALID;
}

/*  @} */
