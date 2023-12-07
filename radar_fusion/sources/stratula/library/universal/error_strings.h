/****************************************************************************\
* Copyright (C) 2017 Infineon Technologies
*
* THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
\****************************************************************************/
#ifndef ERROR_STRINGS_H
#define ERROR_STRINGS_H 1

#include "data_definitions.h"
#include "error_definitions.h"
#include "protocol/protocol_definitions.h"


#define DEFINE_CASE(code) \
    case code:            \
        return #code;     \
        break;


#ifdef __cplusplus
inline
#else
static
#endif
const char *getErrorString(int error)
{
    switch (error)
    {
        DEFINE_CASE(STATUS_HEADER_INCOMPLETE)
        DEFINE_CASE(STATUS_PAYLOAD_INCOMPLETE)
        DEFINE_CASE(STATUS_PAYLOAD_TOO_LONG)
        DEFINE_CASE(STATUS_CRC_ERROR)

        DEFINE_CASE(STATUS_REQUEST_TYPE_INVALID)
        DEFINE_CASE(STATUS_REQUEST_INVALID)

        DEFINE_CASE(STATUS_REQUEST_NOT_IMPLEMENTED)
        DEFINE_CASE(STATUS_REQUEST_WLENGTH_INVALID)
        DEFINE_CASE(STATUS_REQUEST_WVALUE_INVALID)
        DEFINE_CASE(STATUS_REQUEST_WINDEX_INVALID)
        DEFINE_CASE(STATUS_REQUEST_PAYLOAD_INVALID)

        DEFINE_CASE(STATUS_COMMAND_ID_INVALID)
        DEFINE_CASE(STATUS_COMMAND_TYPE_INVALID)
        DEFINE_CASE(STATUS_COMMAND_IMPL_INVALID)
        DEFINE_CASE(STATUS_COMMAND_SUBIF_INVALID)
        DEFINE_CASE(STATUS_COMMAND_FUNCTION_INVALID)

        DEFINE_CASE(E_FAILED)
        DEFINE_CASE(E_INVALID_SIZE)
        DEFINE_CASE(E_INVALID_PARAMETER)
        DEFINE_CASE(E_MISSING_PARAMETER)
        DEFINE_CASE(E_UNEXPECTED_VALUE)
        DEFINE_CASE(E_OUT_OF_BOUNDS)
        DEFINE_CASE(E_OVERFLOW)
        DEFINE_CASE(E_TIMEOUT)
        DEFINE_CASE(E_BUSY)
        DEFINE_CASE(E_ABORTED)

        DEFINE_CASE(E_NOT_IMPLEMENTED)
        DEFINE_CASE(E_NOT_POSSIBLE)
        DEFINE_CASE(E_NOT_AVAILABLE)
        DEFINE_CASE(E_NOT_ALLOWED)
        DEFINE_CASE(E_NOT_INITIALIZED)
        DEFINE_CASE(E_NOT_CONFIGURED)
        DEFINE_CASE(E_NOT_OPEN)

        DEFINE_CASE(E_FW_LOAD_FAILED)
        DEFINE_CASE(E_FW_TIMEOUT_SETUP)
        DEFINE_CASE(E_FW_FUNCTION_ERROR)
        DEFINE_CASE(E_FW_TIMEOUT_COMPLETION)
        DEFINE_CASE(E_FW_UNEXPECTED_RETURN)
        DEFINE_CASE(E_FW_FUNCTION_NOT_STARTED)
        DEFINE_CASE(E_FW_FUNCTION_FAILED)

        DEFINE_CASE(DataError_LowLevelError)
        DEFINE_CASE(DataError_FrameDropped)
        DEFINE_CASE(DataError_FramePoolDepleted)
        DEFINE_CASE(DataError_FrameSizeExceeded)
        DEFINE_CASE(DataError_FrameQueueTrimmed)

        default:
            return "(no error string defined)";
    }
}

#endif /* ERROR_STRINGS_H */
