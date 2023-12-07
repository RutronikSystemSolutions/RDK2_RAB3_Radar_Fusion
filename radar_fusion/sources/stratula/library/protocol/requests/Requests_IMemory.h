#ifndef REQUESTS_IMEMORY_H_
#define REQUESTS_IMEMORY_H_ 1

#include <stdint.h>


/** Writes to device memory.
 *
 *  @param wValue  32bit address HIGH word (use MSB 0xC9 to access remote memory 0xB9000000 range)
 *  @param wIndex  32bit address LOW word
 *  @param wLength byte count
 *  @param payload buffer containing the data to be stored
 *
 *  @return bStatus STATUS_SUCCESS once execution finishes
 *                  propagates error_communication if remote memory access failed
 */
uint8_t Requests_IMemory_write(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);

/** Reads from device memory.
 *
 *  @param wValue  32bit address HIGH word (use MSB 0xC9 to access remote memory 0xB9000000 range)
 *  @param wIndex  32bit address LOW word
 *  @param wLength byte count
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus STATUS_SUCCESS once execution finishes
 *                  propagates error_communication if remote memory access failed
 */
uint8_t Requests_IMemory_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);

/** Streams memory content to remote device.
 *
 *  @param wValue  32bit destination address LOW word
 *  @param wIndex  32bit destination address HIGH word
 *  @param wLength 8
 *  @param payload buffer containing: 32bit source address, 32bit length.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_REQUEST_WLENGTH_INVALID if wLength has incorrect value
 *                  propagates error_communication if parameters were valid but execution failed
 */
uint8_t Requests_IMemory_stream(uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);

#endif /* REQUESTS_IMEMORY_H_ */
