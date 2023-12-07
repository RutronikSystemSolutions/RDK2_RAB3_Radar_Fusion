#ifndef REQUEST_HANDLER_CUSTOM_H_
#define REQUEST_HANDLER_CUSTOM_H_ 1

#include <stdint.h>
#include <universal/protocol/protocol_definitions.h>


/** Forward a write-command to one of the write-command-functions
 *  associated to the commands defined in protocol_definitions_custom.h
 *
 *  @param wValue  wValue identifies the command
 *  Note that there might (or might not) be a command-function-write associated to each one of those values.
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be written (i.e. it could be zero if
 *  				no more data is needed apart from wIndex)
 *  @param payload buffer containing the payload
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is not command-function-write associated to the request defined by wValue
 *  	- bStatus returned by the command-function-write associated to the command
 */
static uint8_t RequestHandlerCustom_write(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload)
{
    return STATUS_REQUEST_NOT_IMPLEMENTED;
}

/** Forward a read-command to one of the read-command-functions
 *  associated to the commands defined in protocol_definitions_custom.h
 *
 *  @param wValue  wValue identifies the command
 *  Note that there might (or might not) be a command-function-read associated to each one of those values.
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be written (i.e. it could be zero if
 *  				no more data is needed apart from wIndex)
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is not command-function-read associated to the request defined by wValue
 *  	- bStatus returned by the command-function-read associated to the command
 */
static uint8_t RequestHandlerCustom_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload)
{
    return STATUS_REQUEST_NOT_IMPLEMENTED;
}


static uint8_t RequestHandlerCustom_transfer(uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    return STATUS_REQUEST_NOT_IMPLEMENTED;
}


#endif /* REQUEST_HANDLER_CUSTOM_H_ */
