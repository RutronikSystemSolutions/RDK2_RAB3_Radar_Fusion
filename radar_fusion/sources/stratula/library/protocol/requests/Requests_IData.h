#ifndef REQUESTS_IDATA_H_
#define REQUESTS_IDATA_H_ 1

#include <platform/interfaces/IData.h>
#include <stdint.h>


uint8_t Requests_IData_write(IData *data, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);
uint8_t Requests_IData_read(IData *data, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);


#endif /* REQUESTS_IDATA_H_ */
