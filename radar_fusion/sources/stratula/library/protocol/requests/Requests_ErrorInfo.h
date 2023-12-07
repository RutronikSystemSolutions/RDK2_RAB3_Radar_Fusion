#ifndef REQUESTS_ERROR_INFO_H_
#define REQUESTS_ERROR_INFO_H_ 1

#include <stdint.h>


uint8_t Requests_ErrorInfo_read(uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);


#endif /* REQUESTS_ERROR_INFO_H_ */
