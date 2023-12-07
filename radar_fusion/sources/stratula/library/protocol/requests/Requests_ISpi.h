#ifndef REQUESTS_ISPI_H_
#define REQUESTS_ISPI_H_ 1

#include <platform/interfaces/ISpi.h>
#include <stdint.h>


/** Write a raw byte stream to an SPI device
 *
 *  @param wValue  device ID (low byte) and buffer element width in bytes (high byte)
 *  @param wIndex
 *  	- LOW byte: 0 for releasing slave select, 1 for keeping slave select
        - HIGH byte: 0 for "operation finished with this call", 1 for "more calls following for this operation"
 *  @param wLength byte count
 *  @param payload byte data
 *
 *  @return bStatus
 *  - STATUS_SUCCESS if parameters were valid and execution successful
 *  - STATUS_REQUEST_NOT_IMPLEMENTED if the HIGH byte of wIndex is not zero.
 *    "more calls following for this operation" is not yet supported.
 *  - Otherwise,  returns the error reported by spi-HAL execution, i.e.
 *    E_INVALID_PARAMETER, STATUS_REQUEST_NOT_IMPLEMENTED, or E_FAILED.
 */
uint8_t Requests_ISpi_write(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);

/** Read a raw byte stream from an SPI device
 *
 *  @param wValue  device ID (low byte) and buffer element width in bytes (high byte)
 *  @param wIndex
 *  	- LOW byte: 0 for releasing slave select, 1 for keeping slave select
        - HIGH byte: 0 for "operation finished with this call", 1 for "more calls following for this operation"
 *  @param wLength byte count
 *  @param payload byte data
 *
 *  @return bStatus
 *  - STATUS_SUCCESS if parameters were valid and execution successful
 *  - STATUS_REQUEST_NOT_IMPLEMENTED if the HIGH byte of wIndex is not zero.
 *    "more calls following for this operation" is not yet supported.
 *  - Otherwise,  returns the error reported by spi-HAL execution, i.e.
 *    E_INVALID_PARAMETER, STATUS_REQUEST_NOT_IMPLEMENTED, or E_FAILED.
 */
uint8_t Requests_ISpi_read(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);

/** Read and write raw byte streams from / to an SPI device in one transfer
 *
 *  @param wValue  device ID (low byte) and buffer element width in bytes (high byte)
 *  @param wIndex
 *  	- LOW byte: 0 for releasing slave select, 1 for keeping slave select
        - HIGH byte: 0 for "operation finished with this call", 1 for "more calls following for this operation"
 *  @param wLengthIn byte count of the input data to write
 *  @param payloadIn byte data to write
 *  @param wLengthOut byte count of the received data, filled by the callee
 *  @param payloadOut buffer, where the received data will be found after the call
 *
 *  @return bStatus
 *  - STATUS_SUCCESS if parameters were valid and execution successful
 *  - STATUS_REQUEST_NOT_IMPLEMENTED if the HIGH byte of wIndex is not zero.
 *    "more calls following for this operation" is not yet supported.
 *  - Otherwise,  returns the error reported by spi-HAL execution, i.e.
 *    E_INVALID_PARAMETER, STATUS_REQUEST_NOT_IMPLEMENTED, or E_FAILED.
 */
uint8_t Requests_ISpi_transfer(ISpi *spi, uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* REQUESTS_ISPI_H_ */
