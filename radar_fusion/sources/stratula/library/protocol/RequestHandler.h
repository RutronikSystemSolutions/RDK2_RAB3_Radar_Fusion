#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_ 1

#include <stdint.h>

#include "commands/ICommands.h"
#include <platform/interfaces/IData.h>
#include <platform/interfaces/IGpio.h>
#include <platform/interfaces/II2c.h>
#include <platform/interfaces/ISpi.h>

/** Constructor
 * - initializes the needed pointer to the different platform-interfaces
 * - calls CommandHandlerRadar_Constructor()
 */
void RequestHandler_Constructor(IGpio *gpio, ISpi *spi, IData *data, II2c *i2c);

/** Register a commands implementation for a component type
 *  @param commands Pointer to the struct containing the implementation
 *  @return true if successfully registered
 */
bool RequestHandler_registerComponentImplementation(const ICommands *commands);

/** Register a commands implementation for a module type
 *  @param commands Pointer to the struct containing the implementation
 *  @return true if successfully registered
 */
bool RequestHandler_registerModuleImplementation(const ICommands *commands);

/** Forward a write-command-request to one of the write-command-functions
 *  associated to the requests defined in protocol_definitions.h
 *
 *  @param bRequest	One of the values defined at protocol_definitions.h, there might be a
 *  command-function-write associated to each one of those values.
 *  @param wValue  wValue (interpreted differently by each command)
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be written (i.e. it could be zero if
 *  				no more data is needed apart from wValue and wIndex)
 *  @param payload buffer containing the payload
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is not command-function-write associated to the bRequest
 *  	- bStatus returned by the command-function-write associated to the bRequest
 */
uint8_t RequestHandler_write(uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
                             uint16_t wLength, const uint8_t *payload);

/** Forward a read-command-request to one of the read-command-functions
 *  associated to the requests defined in protocol_definitions.h
 *
 *  @param bRequest	One of the values defined at protocol_definitions.h, there might be a
 *  command-function-read associated to each one of those values.
 *  @param wValue  wValue (interpreted differently by each command)
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be read (if zero, no payload-data will be sent back to the host)
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is not command-function-read associated to the bRequest
 *  	- bStatus returned by the command-function-read associated to the bRequest
 */
uint8_t RequestHandler_read(uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
                            uint16_t wLength, uint8_t **payload);

/** Forward a transfer-command-request to one of the transfer-command-functions
 *  associated to the requests defined in protocol_definitions.h
 *
 *  @param bRequest	One of the values defined at protocol_definitions.h, there might be a
 *  command-function-transfer associated to each one of those values.
 *  @param wValue  wValue (interpreted differently by each command)
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLengthIn length of payload to be written
 *  @param payloadIn buffer containing the payload
 *  @param wLengthOut length of payload to be sent back (filled by the callee)
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is not command-function-read associated to the bRequest
 *  	- bStatus returned by the command-function-read associated to the bRequest
 */
uint8_t RequestHandler_transfer(uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
                                uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* REQUESTHANDLER_H_ */
