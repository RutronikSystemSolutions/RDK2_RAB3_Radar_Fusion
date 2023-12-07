#ifndef REQUESTS_BOARD_INFO_H_
#define REQUESTS_BOARD_INFO_H_ 1

#include <stdint.h>


uint8_t Requests_BoardInfo_transfer(uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

uint8_t Requests_BoardInfo_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);


#endif /* REQUESTS_BOARD_INFO_H_ */
