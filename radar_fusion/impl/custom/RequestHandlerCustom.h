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

#ifndef REQUEST_HANDLER_CUSTOM_H_
#define REQUEST_HANDLER_CUSTOM_H_ 1

#include <stdint.h>
#include <universal/protocol/protocol_definitions.h>
#include <universal/protocol/protocol_definitions_rdk.h>


/** Forward a write-command to one of the write-command-functions
 *  associated to the commands defined in protocol_definitions_custom.h
 *
 *  @param wValue  wValue identifies the command
 *  Note that there might (or might not) be a command-function-write associated to each one of those values.
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be written (i.e. it could be zero if
 *                  no more data is needed apart from wIndex)
 *  @param payload buffer containing the payload
 *
 *  @return bStatus as follows:
 *      - STATUS_REQUEST_INVALID if there is not command-function-write associated to the request defined by wValue
 *      - bStatus returned by the command-function-write associated to the command
 */
uint8_t RequestHandlerCustom_write(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);


/** Forward a read-command to one of the read-command-functions
 *  associated to the commands defined in protocol_definitions_custom.h
 *
 *  @param wValue  wValue identifies the command
 *  Note that there might (or might not) be a command-function-read associated to each one of those values.
 *  @param wIndex  wIndex (interpreted differently by each command)
 *  @param wLength length of payload to be written (i.e. it could be zero if
 *                  no more data is needed apart from wIndex)
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *      - STATUS_REQUEST_INVALID if there is not command-function-read associated to the request defined by wValue
 *      - bStatus returned by the command-function-read associated to the command
 */
uint8_t RequestHandlerCustom_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);


uint8_t RequestHandlerCustom_transfer(uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* REQUEST_HANDLER_CUSTOM_H_ */
